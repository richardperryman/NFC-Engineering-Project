const QUERY_GET_WITH_AUTH = 'SELECT * FROM sbacsDb.Authenticators WHERE Auth_Id = ?';
const QUERY_GET_WITH_IDENTITY = 'SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.IdentityToAuth as i, sbacsDb.Authenticators as a WHERE i.Identity_Id = ?';
const QUERY_GET_WITH_USER = 'SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.Identities AS i, sbacsDb.IdentityToAuth as ita, sbacsDb.Authenticators as a WHERE i.User_Id = ? AND i.Identity_Id = ita.Identity_Id AND ita.Auth_Id = a.Auth_Id';
const QUERY_PUT = 'INSERT INTO sbacsDb.Authenticators (AuthType, AuthKey, AuthSalt) VALUES (?, ?, ?)';
const QUERY_DELETE = 'DELETE FROM sbacsDb.Authenticators WHERE Auth_Id = ?';
const QUERY_POST = 'UPDATE sbacsDb.Authenticators SET AuthType = ?, AuthKey = ?, AuthSalt = ? WHERE Auth_Id = ?';


var url = require('url');
var mysql = require('mysql');

var db;


module.exports = {

handleRequest : function(req,res,db_helper) {
	console.log('Reached Authenticator Controller');
	
	db = db_helper;
	
	if(req.method === 'GET'){
		handleGet(req,res);
	} else if (req.method === 'POST') {
		handlePost(req,res);
	} else if (req.method === 'PUT') {
		handlePut(req,res);
	} else if (req.method === 'DELETE') {
		handleDelete(req,res);
	} else {
		// Change to 405, with Allow header later
		res.writeHead(500);
		res.write('Method not supported');
		res.end();
	}
}

};


function handleGet(req,res){
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var auth_id = parsedRequest.query['auth_id'];
	var identity_id = parsedRequest.query['identity_id'];
	var user_id = parsedRequest.query['user_id'];
	
	// TODO: Still need to construct query
	if(auth_id == undefined){
		if(identity_id == undefined){
			if(user_id == undefined){
				var inserts = ['%'];
				queryString = mysql.format(QUERY_GET_WITH_AUTH,inserts);
			} else {
				var inserts = [user_id];
				queryString = mysql.format(QUERY_GET_WITH_USER,inserts);
			}
			
		} else {
			var inserts = [identity_id];
			queryString = mysql.format(QUERY_GET_WITH_IDENTITY,inserts);
		}
	} else {
		var inserts = [auth_id];
		queryString = mysql.format(QUERY_GET_WITH_AUTH,inserts);
	}
	
	console.log(queryString);
	
	// Execute query, return needed results
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			var formattedOut = db.formatDbOutput(rows,fields);
			res.writeHead(200);
			res.write(formattedOut);
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
	res.writeHead(200);
	res.write('Generic response');
	res.end();
}

function handlePut(req,res){
	res.writeHead(200);
	res.write('Generic response');
	res.end();
}

function handleDelete(req,res){
	res.writeHead(200);
	res.write('Generic response');
	res.end();
}