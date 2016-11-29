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
	// TODO Write the authenticator set GET method
}

function handlePost(req,res){
	var parsedRequest = url.parse(req.url, true);
	
	// Extract information that is needed
	var lock_id = parsedRequest.query['lock_id'];
	var given_auth = parsedRequest.query['auth'];
	
	if(lock_id == undefined){
		console.log('No lock provided for access');
		res.writeHead(500);
		res.write('Lock_Id not provided');
		res.end();
		return;
	} else if (given_auth == undefined){
		console.log('No authenticators provided for access');
		res.writeHead(500);
		res.write('Authenticators not provided');
		res.end();
		return;
	}
	var inserts = [lock_id];
	var queryString = mysql.format(QUERY_GET_NEEDED_AUTH, inserts);
	
	// Execute query to get needed auth
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			// TODO: Check the needed against given, then do necessary
			var needed_auth = extractAuthenticators(rows,fields);
			if(checkAuth(given_auth, needed_auth)){
				res.writeHead(200);
				res.write('SuccessCode');
				res.end();
			} else {
				res.writeHead(401);
				res.write('Access Denied');
				res.end();
			}
		} else {
			// Handle error
			console.log('Error with DB');
			res.writeHead(500);
			res.write('An error occurred.');
			res.end();
		}
	});
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
function extractAuthenticators(rows,fields) {
	// TODO: Implement this function
	
	return 'blah';
}