var port = process.env.PORT || 8081,
    http = require('http'),
	url = require('url'),
    fs = require('fs');

var log = function(entry) {
    fs.appendFileSync('/tmp/sample-app.log', new Date().toISOString() + ' - ' + entry + '\n');
};

var db_helper = require('./extensions/db_helper.js');
// TODO: Decide if this is best here, or in the controllers that use it

var server = http.createServer(serverRequest);

// Set up external handlers
var userHandler = require('./controllers/userController.js');
var accessManagementHandler = require('./controllers/accessManagementController.js');
var accessHandler = require('./controllers/accessController.js');
var identityHandler = require('./controllers/identityController.js');
var authenticatorHandler = require('./controllers/authenticatorController.js');
var lockHandler = require('./controllers/lockController.js');
var loginHandler = require('./controllers/loginController.js');
var signupHandler = require('./controllers/signupController.js');
var notificationHandler = require('./controllers/notificationController.js');

function serverRequest(req, res) {
	var parsedRequest = url.parse(req.url, true);
	
	res.setHeader('Access-Control-Allow-Origin','*');
	
	if(parsedRequest.pathname === '/users'){
		userHandler.handleRequest(req,res,db_helper);
	} else if (parsedRequest.pathname === '/identities'){
		identityHandler.handleRequest(req,res,db_helper);
	} else if (parsedRequest.pathname === '/authenticators'){
		authenticatorHandler.handleRequest(req,res,db_helper);
	} else if (parsedRequest.pathname === '/locks'){
		lockHandler.handleRequest(req,res,db_helper);
	} else if (parsedRequest.pathname === '/access'){
		accessHandler.handleRequest(req,res,db_helper);
	} else if (parsedRequest.pathname === '/access/management'){
		accessManagementHandler.handleRequest(req,res,db_helper);
	} else if (parsedRequest.pathname === '/login'){
		loginHandler.handleRequest(req,res,db_helper);
	} else if (parsedRequest.pathname === '/signup'){
		signupHandler.handleRequest(req,res);
	} else if (parsedRequest.pathname === '/notifications'){
		notificationHandler.handleRequest(req,res,db_helper);
	} else {
		res.writeHead(200);
		res.write('Generic response');
		res.end();
	}
	
	
}

// Listen on port 3000, IP defaults to 127.0.0.1
server.listen(port);

// Put a friendly message on the terminal
console.log('Server running at http://127.0.0.1:' + port + '/');

var https = require('https');
var securePort = process.env.HTTPS_PORT || 443;

var secureServer = https.createServer(secureRequest);

function secureRequest(req,res){
	res.writeHead(200);
	res.write('Secure response');
	res.end();
}