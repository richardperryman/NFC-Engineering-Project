var port = process.env.PORT || 3000,
    http = require('http'),
	url = require('url'),
    fs = require('fs');

var log = function(entry) {
    fs.appendFileSync('/tmp/sample-app.log', new Date().toISOString() + ' - ' + entry + '\n');
};

// Set up connection to DB
var mysql = require('mysql');
var pool = mysql.createPool({
	host : 'sbacsdbinstance.cpbvqdyj5xzw.us-west-2.rds.amazonaws.com',
	user : 'craigshorrocks',
	password : 'project2016jrc',
	port: 3306
});

var server = http.createServer(serverRequest);

// Set up external handlers
var userHandler = require('./controllers/userController.js');
var accessManagementHandler = require('./controllers/accessManagementController.js');
var accessHandler = require('./controllers/accessController.js');
var identityHandler = require('./controllers/identityController.js');
var authenticatorHandler = require('./controllers/authenticatorController.js');
var lockHandler = require('./controllers/lockController.js');

function serverRequest(req, res) {
	var parsedRequest = url.parse(req.url, true);
	
	if(parsedRequest.pathname === '/users'){
		userHandler.handleRequest(req,res);
	} else if (parsedRequest.pathname === '/identities'){
		identityHandler.handleRequest(req,res);
	} else if (parsedRequest.pathname === '/authenticators'){
		authenticatorHandler.handleRequest(req,res);
	} else if (parsedRequest.pathname === '/locks'){
		lockHandler.handleRequest(req,res);
	} else if (parsedRequest.pathname === '/access'){
		accessHandler.handleRequest(req,res);
	} else if (parsedRequest.pathname === '/access/management'){
		accessManagementHandler.handleRequest(req,res);
	}
	
	res.writeHead(200);
	res.write('Generic response');
	res.end();
}

// TODO: Move to other module, where more useful
function getDbResponse(pool, callback){
	// Get a connection to DB from the pool
	pool.getConnection(function(err,connection){
		// Make a query with that connection
		connection.query('SELECT * FROM innodb.Users', function(err,rows,fields){
			// As soon as we have results, we can release the connection back to pool
			connection.release();
			if(!err){
				console.log('Output ', rows);
				return callback(rows);	// Send results to the callback function
			} else {
				console.log('Error');
			}
		});
	});
}

// TODO: Move to other module, where more useful
function formatUsersDbResponse(result) {
	// TODO: Make formatting prettier (maybe use xml or json)
	var string = 'Name:   Rights: ';
	for(i=0;i<result.length;i++){
		string += '\n' + result[i].Name + '  ' + result[i].Rights;
	}
	return string;
}

// Listen on port 3000, IP defaults to 127.0.0.1
server.listen(port);

// Put a friendly message on the terminal
console.log('Server running at http://127.0.0.1:' + port + '/');

