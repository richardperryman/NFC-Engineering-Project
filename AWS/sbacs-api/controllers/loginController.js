
const QUERY_USER_PASSWORD = 'SELECT u.User_Id, a.AuthType, a.AuthKey, a.AuthSalt FROM sbacsDb.Users as u, sbacsDb.Authenticators as a, sbacsDb.IdentityToAuth as ita, sbacsDb.Identities as i WHERE ita.Auth_Id = a.Auth_Id AND ita.Identity_Id = i.Identity_Id AND i.User_Id = u.User_Id AND u.Name = ?';

var url = require('url');
var auth_helper = require('../extensions/auth_helper.js');
var crypt = require('../extensions/crypto_helper.js');

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
					if(hashedGiven.toString() == expectedPassword.value){
						var key = auth_helper.getUserKey(expectedPassword.id);
						if(key == undefined){
							key = generateKey(expectedPassword.id);
						}
						res.writeHead(200);
						res.write(key);
						res.end();
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

// Returns the expected password as {id:val,type:val,value:val,salt:val}
function findPassword(rows){
	for(var i=0;i<rows.length;i++){
		var auth = {id:rows[i]['User_Id'],type:rows[i]['AuthType'],value:rows[i]['AuthKey'],salt:rows[i]['AuthSalt']};
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
