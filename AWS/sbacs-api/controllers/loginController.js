
const QUERY_USER_PASSWORD = 'SELECT u.User_Id, ita.Identity_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.Users as u, sbacsDb.Authenticators as a, sbacsDb.IdentityToAuth as ita, sbacsDb.Identities as i WHERE ita.Auth_Id = a.Auth_Id AND ita.Identity_Id = i.Identity_Id AND i.User_Id = u.User_Id AND u.Name = ?';

var url = require('url');
var mysql = require('mysql');
var auth_helper = require('../extensions/auth_helper.js');
var crypt = require('../extensions/crypto_helper.js');
var request = require('request');

var db;

module.exports = {

handleRequest : function(req,res,db_helper){
	console.log('Reached Login Controller');
	
	db = db_helper;

	if (req.method === 'POST') {
		handlePost(req,res);
	} else {
		// Change to 405, with Allow header later
		res.writeHead(500);
		res.write('Method not supported');
		res.end();
	}
}

};

function handlePost(req,res){
	// Get body of message
	var body = [];
	req.on('data', function(chunk) {
		body.push(chunk);
	}).on('end', function () {
		body = Buffer.concat(body).toString();
		
		// Parse body into object
		var login = parseLoginBody(body);
		
		// Get the expected password for the given user name
		var inserts = [login.username];
		var queryString = mysql.format(QUERY_USER_PASSWORD,inserts);
		
		db.performQuery(queryString, function(err,rows,fields){
			if(!err){
				var expectedPassword = findPassword(rows);
				if(expectedPassword == undefined){
					res.writeHead(401);
					res.write('User not found');
					res.end();
				} else {
					var hashedGiven = new crypt.encryptedAuth(login.password,expectedPassword.salt);
					if(hashedGiven.secret.toString() == expectedPassword.value){
						auth_helper.getUserKey(expectedPassword.id,function(key){
							if(key == undefined){
								key = auth_helper.generateUserKey(expectedPassword.id);
							} else {
								// Print expiry time (TODO: remove this)
								console.log('Key expires at: ' + key.expiry);
								key = key.value;
							}
							key = {user:expectedPassword.id,key:key};
							res.writeHead(200);
							res.write(JSON.stringify(key));
							res.end();
							// Set nfc authenticator to key 
							saveNfcKey(key.key,expectedPassword.identity);
						});
					} else {
						res.writeHead(401);
						res.write('Invalid Username/password');
						res.end();
					}
				}
			} else {
				// Handle error
				console.log('Error with DB');
				res.writeHead(500);
				res.write('An error occurred.');
				res.end();
			}
		});
		
	});

}

// key contains {user,key}
function saveNfcKey(key,identity_id){
	// Make PUT request to /authenticators to save key
	var query = '?authType=nfc&identity_id=';
	query += identity_id;
	var http = require('http');
	var options = {
	  hostname: '127.0.0.1',
	  port: 8081,
	  path: ('/authenticators' + query),
	  method: 'PUT',
	  headers: {
		  'hmac-user': '10outta13',
	  }
	};
	var req = http.request(options, function(res) {

	});
	// write data to request body
	req.write(JSON.stringify(key));
	req.end();
}

// Returns the expected password as {id:val,type:val,value:val,salt:val}
function findPassword(rows){
	for(var i=0;i<rows.length;i++){
		var auth = {id:rows[i]['User_Id'],identity:rows[i]['Identity_Id'],type:rows[i]['AuthType'],value:rows[i]['AuthKey'],salt:rows[i]['AuthSalt']};
		if(auth.type == 'Password' || auth.type == 'password'){
			return auth;
		}
	}
}


function parseLoginBody(body){
	// Should be in format {username:val,password:val}
	var obj = JSON.parse(body);
	
	return obj;
}
