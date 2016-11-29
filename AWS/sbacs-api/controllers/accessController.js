// Note that mysql.format(query, values), only works where values
// are strings, and puts the value with single quotes around it

const QUERY_GET_NEEDED_AUTH = 'SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.Authenticators as a, sbacsDb.IdentityToAuth as ita, sbacsDb.Registrations as r WHERE r.Lock_Id = ? AND r.Identity_Id = ita.Identity_Id AND ita.Auth_Id = a.Auth_Id';
const QUERY_GET_LOCK_KEY = 'SELECT lockKey FROM Locks WHERE Lock_Id = ?';


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
					res.write('SuccessCode');
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
	
	return 'blah';
}

// Checks if the given and needed authenticators are equal
//	 returns true if equal, otherwise false
function checkAuth(given, needed){
	// TODO: Implement this function
	
	if(given === needed){
		return true;
	} else {
		return false;
	}
}

// Extracts the authenticators from DB into useful format
function extractAuthenticatorsFromDb(rows,fields) {
	// TODO: Implement this function
	
	return 'blah';
}

// Extracts the authenticators from Body info useful format
function extractAuthenticatorsFromBody(body){
	// TODO: Implement this function
	
	return 'blah';
}