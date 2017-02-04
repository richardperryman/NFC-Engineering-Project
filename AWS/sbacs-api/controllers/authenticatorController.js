const QUERY_GET_ALL = 'SELECT * FROM sbacsDb.Authenticators';
const QUERY_GET_WITH_AUTH = 'SELECT * FROM sbacsDb.Authenticators WHERE Auth_Id = ?';
const QUERY_GET_WITH_IDENTITY = 'SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.IdentityToAuth as i, sbacsDb.Authenticators as a WHERE i.Identity_Id = ? AND i.Auth_Id = a.Auth_Id';
const QUERY_GET_WITH_USER = 'SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.Identities AS i, sbacsDb.IdentityToAuth as ita, sbacsDb.Authenticators as a WHERE i.User_Id = ? AND i.Identity_Id = ita.Identity_Id AND ita.Auth_Id = a.Auth_Id';
const QUERY_PUT = 'INSERT INTO sbacsDb.Authenticators (AuthType, AuthKey, AuthSalt) VALUES (?, ?, ?)';
const QUERY_PUT_IDENTITY = 'INSERT INTO sbacsDb.IdentityToAuth (Identity_Id, Auth_Id) VALUES (?, ?)';
const QUERY_DELETE_IDENTITY = 'DELETE FROM sbacsDb.IdentityToAuth WHERE Auth_Id = ?';
const QUERY_DELETE = 'DELETE FROM sbacsDb.Authenticators WHERE Auth_Id = ?';
const QUERY_POST = 'UPDATE sbacsDb.Authenticators SET AuthType = ?, AuthKey = ? WHERE Auth_Id = ?';
const QUERY_GET_SALT_WITH_AUTH = 'SELECT AuthSalt FROM sbacsDb.Authenticators WHERE Auth_Id = ?';


var url = require('url');
var mysql = require('mysql');
var crypt = require('../extensions/crypto_helper.js');
var auth_helper = require('../extensions/auth_helper.js');

var db;


module.exports = {

handleRequest : function(req,res,db_helper) {
	console.log('Reached Authenticator Controller');
	
	db = db_helper;
	
	auth_helper.authenticate(req,function(authenticated){
		if(authenticated){
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
		} else {
			res.writeHead(401);
			res.write('Authentication invalid or not provided');
			res.end();
		}
	});
	
	
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
				queryString = QUERY_GET_ALL;
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
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var auth_id = parsedRequest.query['auth_id'];
	var authType = parsedRequest.query['authType'];
	req.on('data', function(chunk){
		var authValue = chunk.toString();
		if(auth_id == undefined || authType == undefined || authValue == undefined){
			console.log('Invalid input');
			res.writeHead(400);
			res.write('Not all parameters met.');
			res.end();
			return;
		}
		
		var saltedValue = getSaltedValue(authValue,auth_id,function(saltedAuthValue){
			var inserts = [authType,saltedAuthValue,auth_id];
			queryString = mysql.format(QUERY_POST,inserts);
			// Execute query, return needed results
			db.performQuery(queryString, function(err,rows,fields){
				if(!err){
					var formattedOut = 'Successfully updated row';
					res.writeHead(200);
					res.write(formattedOut);
					res.end();
				} else {
					// Handle error
					console.log('Error with DB');
					res.writeHead(500);
					res.write('Unable to complete request.');
					res.end();
				}
			});
		});
	});
}

function getSaltedValue(authValue,auth_id,callback){
	var inserts = [auth_id];
	var queryString = mysql.format(QUERY_GET_SALT_WITH_AUTH,inserts);
	
	// Execute query, return needed results
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			var salt = rows[0]['AuthSalt'];
			var encryptedInfo = new crypt.encryptedAuth(authValue,salt);
			console.log('Value: ' + encryptedInfo.secret);
			return callback(encryptedInfo.secret);
		} else {
			// Handle error
			console.log('Error with DB');
			res.writeHead(500);
			res.write('Unable to complete request.');
			res.end();
		}
	});
}

function handlePut(req,res){
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var authType = parsedRequest.query['authType'];
	var identity_id = parsedRequest.query['identity_id'];
	req.on('data', function(chunk){
		var authValue = chunk.toString();
		if(authType == undefined || authValue == undefined || identity_id == undefined){
			console.log('Invalid input');
			res.writeHead(400);
			res.write('Not all parameters met.');
			res.end();
			return;
		}
		// Make this an actual salt
		var encryptedInfo = new crypt.encryptedAuth(authValue);
		var inserts = [authType,encryptedInfo.secret,encryptedInfo.salt];
		queryString = mysql.format(QUERY_PUT,inserts);
		// Execute query, return needed results
		db.performQuery(queryString, function(err,rows,fields){
			if(!err){
				var formattedOut = 'ID of created row: ' + rows.insertId;
				inserts = [identity_id,rows.insertId];
				queryString = mysql.format(QUERY_PUT_IDENTITY,inserts);
				db.performQuery(queryString, function(err,rows,fields){
					if(!err){
						res.writeHead(200);
						res.write(formattedOut);
						res.end();
					} else {
						// Handle error
						console.log('Error with DB');
						res.writeHead(500);
						res.write('Unable to complete request.');
						res.end();
					}
				});
			} else {
				// Handle error
				console.log('Error with DB');
				res.writeHead(500);
				res.write('Unable to complete request.');
				res.end();
			}
		});
	});
	
}

function handleDelete(req,res){
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var auth_id = parsedRequest.query['auth_id'];
	if(auth_id == undefined){
		console.log('Invalid input');
		res.writeHead(400);
		res.write('Not all parameters met.');
		res.end();
		return;
	}
	var inserts = [auth_id];
	queryString = mysql.format(QUERY_DELETE_IDENTITY,inserts);
	// Execute query, return needed results
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			queryString = mysql.format(QUERY_DELETE,inserts);
			db.performQuery(queryString, function(err,rows,fields){
				if(!err){	
					var formattedOut = 'Successfully deleted row';
					res.writeHead(200);
					res.write(formattedOut);
					res.end();
				} else {
					// Handle error
					console.log('Error with DB');
					res.writeHead(500);
					res.write('Unable to complete request.');
					res.end();
				}
			});
		} else {
			// Handle error
			console.log('Error with DB');
			res.writeHead(500);
			res.write('Unable to complete request.');
			res.end();
		}
	});
}