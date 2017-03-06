


var url = require('url');


module.exports = {

handleRequest : function(req,res){
	console.log('Reached SignUp Controller');

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

	// Get user name and passwrod from body
	var body = [];
	req.on('data', function(chunk) {
		body.push(chunk);
	}).on('end', function () {
		body = Buffer.concat(body).toString();
		
		var info = parseBody(body);
		// Create user
		createUser(info.username,function(user_id,err){
			if(!err){
				// Create Identity
				createIdentity(user_id,function(identity_id,err){
					if(!err){
						// Create Authenticator
						createAuthenticator(identity_id,info.password,function(err){
							if(!err){
								// TODO: Return success message
								res.writeHead(200);
								res.write('User created successfully');
								res.end();
							} else {
								// Handle error
								console.log('Error with creating Auth');
								res.writeHead(500);
								res.write('An error occurred.');
								res.end();
							}
						});
					} else {
						// Handle error
						console.log('Error with creating Identity');
						res.writeHead(500);
						res.write('An error occurred.');
						res.end();
					}
				});
			} else {
				// Handle error
				console.log('Error with creating User');
				res.writeHead(500);
				res.write('An error occurred. Likely a duplicate username');
				res.end();
			}
		});
	});
}

function createUser(username,callback){
	var query = '?rights=customer&name=';
	query += username;
	var http = require('http');
	var options = {
	  hostname: '127.0.0.1',
	  port: 8081,
	  path: ('/users' + query),
	  method: 'PUT',
	  headers: {
		  'hmac-user': '10outta13',
	  }
	};
	var req = http.request(options, function(res) {
		//Check if response is bad
		if(res.statusCode != '200'){
			return callback('','Error');
		}
		// Extract user id from response
		var body = [];
		res.on('data', function(chunk) {
			body.push(chunk);
		}).on('end', function () {
			body = Buffer.concat(body).toString();
			var user_id = body.replace(/[^0-9]/g, '');
			return callback(user_id);
		});
		
	});
	req.end();
}

function createIdentity(userId,callback){
	var query = '?name=default&user_id=';
	query += userId;
	var http = require('http');
	var options = {
	  hostname: '127.0.0.1',
	  port: 8081,
	  path: ('/identities' + query),
	  method: 'PUT',
	  headers: {
		  'hmac-user': '10outta13',
	  }
	};
	var req = http.request(options, function(res) {
		//Check if response is bad
		if(res.statusCode != '200'){
			return callback('','Error');
		}
		
		// Extract identity id from response
		var body = [];
		res.on('data', function(chunk) {
			body.push(chunk);
		}).on('end', function () {
			body = Buffer.concat(body).toString();
			var identity_id = body.replace(/[^0-9]/g, '');
			return callback(identity_id);
		});
	});
	req.end();
}

function createAuthenticator(identityId,password,callback){
	var query = '?authType=password&identity_id=';
	query += identityId;
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
		//Check if response is bad
		if(res.statusCode != '200'){
			return callback('','Error');
		}
		return callback();
	});
	// write data to request body
	req.write(password);
	req.end();
}

// Should be formatted as {"username":"val","password":"val"}
function parseBody(body){
	var result = JSON.parse(body);
	
	return result;
}