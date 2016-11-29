// Note that mysql.format(query, values), only works where values
// are strings, and puts the value with single quotes around it

const QUERY_GET_NEEDED_AUTH = 'SELECT r.Identity_Id, a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.Authenticators as a, sbacsDb.IdentityToAuth as ita, sbacsDb.Registrations as r WHERE r.Lock_Id = ? AND r.Identity_Id = ita.Identity_Id AND ita.Auth_Id = a.Auth_Id';


var url = require('url');
var mysql = require('mysql');

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
			// Send results back
			res.writeHead(200);
			res.write(minimal_auth_set);
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

// Gets the minimal set of authenticators needed for the given list
function getMinimalAuthSet(all_auth) {
	// TODO: Implement this function
	
	// Make a set of authTypes from given list
	// Return that set of authTypes
	
	return 'blah';
}

// Checks if the given and needed authenticators are equal
//	 returns true if equal, otherwise false
function checkAuth(given, needed){
	// TODO: Implement this function
	
	// For each needed list:
		// Get type of needed
		// Check that given has one element of same type
		// Check that given and needed values are same
		// Else on either, return false
	// End for
	
	// Return true if made it out of loop
	
	if(given === needed){
		return true;
	} else {
		return false;
	}
}

// Extracts the authenticators from DB into useful format
function extractAuthenticatorsFromDb(rows,fields) {
	// TODO: Currently only works correctly if there is just one Identity_Id
		// Make it work with multiple somehow
	
	var auth = [];
	for(var i=0;i<rows.length;i++){
		auth[i] = {id:rows[i]['Identity_Id'],type:rows[i]['AuthType'],value:rows[i]['AuthKey'],salt:rows[i]['AuthSalt']};
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