// Note that mysql.format(query, values), only works where values
// are strings, and puts the value with single quotes around it

const QUERY_GET_NEEDED_AUTH = 'SELECT r.Reg_Id, r.UseCount, r.Identity_Id, a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.Authenticators as a, sbacsDb.IdentityToAuth as ita, sbacsDb.Registrations as r WHERE r.Lock_Id = ? AND r.Identity_Id = ita.Identity_Id AND ita.Auth_Id = a.Auth_Id';
const QUERY_DELETE_REG = 'DELETE FROM sbacsDb.Registrations WHERE Reg_Id=?';
const QUERY_UPDATE_COUNT = 'UPDATE sbacsDb.Registrations SET UseCount=? WHERE Reg_Id=?';

var url = require('url');
var mysql = require('mysql');
var crypt = require('../extensions/crypto_helper.js');
var count = -1;
var reg_id = -1;

var db;

module.exports = {

handleRequest : function(req,res,db_helper) {
	console.log('Reached Access Controller');
	
	db = db_helper;
	
	// See what type of request was made
	if(req.method === 'GET'){
		handleGet(req,res);
	} else if (req.method === 'POST'){
		handlePost(req,res);
	} else {
		// Change to 405, with Allow header later
		res.writeHead(500);
		res.write('Method not supported');
		res.end();
	}
}

};


function handleGet(req,res) {
	var parsedRequest = url.parse(req.url, true);
	
	var lock_id = parsedRequest.query['lock_id'];
	if(lock_id == undefined){
		console.log('No lock id provided');
		res.writeHead(500);
		res.write('Lock_Id not provided');
		res.end();
		return;
	}
	
	var inserts = [lock_id];
	var queryString = mysql.format(QUERY_GET_NEEDED_AUTH, inserts);
	// Execute query to get needed auth
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			// Extract authenticator info
			var needed_auth = extractAuthenticatorsFromDb(rows,fields);
			var minimal_auth_set = getMinimalAuthSet(needed_auth);
			
			var auth_set_string = getAuthSetString(minimal_auth_set);
			// Send results back
			res.writeHead(200);
			res.write(auth_set_string);
			res.end();
		} else {
			// Handle error
			console.log('Error with DB');
			res.writeHead(500);
			res.write('An error occurred.');
			res.end();
		}
	});
}

function handlePost(req,res){
	var parsedRequest = url.parse(req.url, true);
	
	// Extract information that is needed
	var lock_id = parsedRequest.query['lock_id'];
	var lock_key = parsedRequest.query['lock_key'];
	
	if(lock_id == undefined){
		console.log('No lock provided for access');
		res.writeHead(500);
		res.write('Lock_Id not provided');
		res.end();
		return;
	} else if (lock_key == undefined){
		console.log('No key provided for access');
		res.writeHead(500);
		res.write('Lock key not provided');
		res.end();
		return;
	}
	
	var inserts = [lock_id];
	var queryString = mysql.format(QUERY_GET_NEEDED_AUTH, inserts);
	// Execute query to get needed auth
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			// Check the needed against given, then do necessary
			var needed_auth = extractAuthenticatorsFromDb(rows,fields);
			
			// Get body and process Authenticators
			var body = [];
			req.on('data', function(chunk) {
				body.push(chunk);
			}).on('end', function () {
				body = Buffer.concat(body).toString();
				
				
				var given_auth = extractAuthenticatorsFromBody(body);
				// Check given and needed authenticators
				if(checkAuth(given_auth, needed_auth)){
					if(count > 0){
						count--;
						saveNewCount();
					}
					res.writeHead(200);
					res.write(lock_key);
					res.end();
				} else {
					res.writeHead(401);
					res.write('Access Denied');
					res.end();
				}
			});
		} else {
			// Handle error
			console.log('Error with DB');
			res.writeHead(500);
			res.write('An error occurred.');
			res.end();
		}
	});
}

function saveNewCount(){
	if(count == 0){
		var inserts = [reg_id];
		var queryString = mysql.format(QUERY_DELETE_REG,inserts);
		db.performQuery(queryString, function(err,rows,fields){
			if(!err){
				console.log('Expired registration deleted');
			} else {
				console.log('Error deleting registration');
			}
			count = -1;
			reg_id = -1;
		});
	} else {
		// TODO: If greater than 0, update the db's count
		var inserts = [count,reg_id];
		var queryString = mysql.format(QUERY_UPDATE_COUNT,inserts);
		db.performQuery(queryString, function(err,rows,fields){
			if(!err){
				console.log('Updated registration count');
			} else {
				console.log('Error updating registration count');
			}
			count = -1;
			reg_id = -1;
		});
		
	}
}

// Gets the minimal set of authenticators needed for the given list
function getMinimalAuthSet(all_auth) {
	// Get list of unique identities
	var identities = [];
	for(var i=0;i<all_auth.length;i++){
		var auth = all_auth[i];
		if(!arrayContains(identities,auth.id)){
			identities.push(auth.id);
		}
	}
	// For each identity, get the set of authenticator types needed
	var final_sets = [];
	for(var i=0;i<identities.length;i++){
		var id = identities[i];
		var id_set = getAuthSetForIdentity(all_auth,id);
		// Add the identity's set to the full set
		final_sets.push(id_set);
	}
	return final_sets;
}

// Get string representation of auth set
function getAuthSetString(auth_set){
	return JSON.stringify(auth_set);
}

// Gets the set of auth types needed for an identity
function getAuthSetForIdentity(all_auth,id){
	var auth_set = [];
	for(var i=0;i<all_auth.length;i++){
		var auth = all_auth[i];
		if(auth.id == id){
			auth_set.push(auth.type);
		}
	}
	return auth_set;
}

// Checks if the given and needed authenticators are equal
//	 returns true if equal, otherwise false
function checkAuth(given, needed){
	// Get list of unique identities
	var identities = [];
	for(var i=0;i<needed.length;i++){
		var auth = needed[i];
		if(!arrayContains(identities,auth.id)){
			identities.push(auth.id);
		}
	}
	// Check for each identity, are any of them met
	for(var i=0;i<identities.length;i++){
		var id = identities[i];
		if(checkAuthForId(given,needed,id)){
			return true;
		}
	}
	return false;
}

// Check that for a given identity, the given authenticators meet requirements
function checkAuthForId(given,needed,id){
	// For each authenticator required by this identity,
		// check that the given list contains it
	for(var i=0;i<needed.length;i++){
		var neededAuth = needed[i];
		if(id == neededAuth.id && neededAuth.type.toLowerCase() != 'password'){
			if(!checkGivenContains(given,neededAuth)){
				return false;
			}
		}
	}
	return true;
}

// Check that a single required authenticator is in the given list
function checkGivenContains(given,neededAuth){
	// For each given auth, if it matches the type, the value must also match
	// If value matches, return true. If we make it through without finding it, return false
	for(var i=0;i<given.length;i++){
		givenAuth = given[i];
		if(givenAuth.type == neededAuth.type){
			var encryptedInfo = new crypt.encryptedAuth(givenAuth.value,neededAuth.salt);
			if(encryptedInfo.secret.toString() == neededAuth.value){
				count = neededAuth.count;
				reg_id = neededAuth.reg;
				return true;
			}
		}
	}
	return false;
}

function getBytesFromString(string){
	var bytes=[];
	for(var i=0;i<string.length;i++){
		bytes.push(string.charCodeAt(i));
	}
	return bytes;
}

// Extracts the authenticators from DB into useful format
function extractAuthenticatorsFromDb(rows,fields) {
	// Extracts all rows of the table as is and turns it into array
	var auth = [];
	for(var i=0;i<rows.length;i++){
		auth[i] = {id:rows[i]['Identity_Id'],type:rows[i]['AuthType'],value:rows[i]['AuthKey'],salt:rows[i]['AuthSalt'],reg:rows[i]['Reg_Id'],count:rows[i]['UseCount']};
	}

	return auth;
}

// Extracts the authenticators from Body info useful format
	// Body will be JSON, following Swagger definition
function extractAuthenticatorsFromBody(body){
	// Format will look like: 
		//'[{"type":"password","value":"blah"},{"type":"nfc","value":"blah2"}]'
	var auth = JSON.parse(body);
	
	return auth;
}

// Apparently not all versions of Node have an Array.include, so writing my own
function arrayContains(array,element) {
	for(var i=0;i<array.length;i++){
		e = array[i];
		if(e == element){
			return true;
		}
	}
	return false;
}