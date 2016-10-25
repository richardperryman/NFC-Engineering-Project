var port = process.env.PORT || 3000,
    http = require('http'),
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

function serverRequest(req, res) {
	if (req.method === 'GET') { 
		getDbResponse(pool, function(result){
			console.log('Output2 ', result);
			var outString = formatUsersDbResponse(result);
			res.writeHead(200);
			res.write(outString);
			res.end();
		});
	}
	// Note for myself to observe when a response is sent
	res.on('finish', function() {
		console.log('Response sent');
	});	
}

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

// TODO: Only does a single line row for now
function formatUsersDbResponse(result) {
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

