// Note that mysql.format(query, values), only works where values
// are strings, and puts the value with single quotes around it

const QUERY_GET_REGISTRATIONS = 'SELECT * FROM sbacsDb.Registrations WHERE Lock_Id = ?';
const QUERY_GET_NEEDED_AUTH = 'SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.Authenticators as a, sbacsDb.IdentityToAuth as ita WHERE ita.Identity_Id = ? AND a.Auth_Id = ita.Auth_Id';
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
	} else {
		// Return generic message
	}
}
// Note that to add a more functions, use a comma

};


function handleGet(req,res) {
	
	// TODO: Extract information that is needed
	var lock_id;
	var identity_id;
	var needed_auth;
	var given_auth;
	var lock_key;
	
	// Use the information
	
	
	res.writeHead(200);
	res.write('SuccessCode');
	res.end();
}