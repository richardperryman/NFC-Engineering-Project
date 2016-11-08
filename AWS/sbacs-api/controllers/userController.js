
const QUERY_GET_WITH_ID = 'SELECT * FROM sbacsDb.Users WHERE User_Id = ?';
const QUERY_GET_WITHOUT_ID = 'SELECT * FROM sbacsDb.Users WHERE Name like ? AND Rights like ?';
const QUERY_PUT = 'INSERT INTO sbacsDb.Users (Name, Rights) VALUES (?, ?)';
const QUERY_DELETE = 'DELETE FROM sbacsDb.Users WHERE User_Id = ?';
const QUERY_POST = 'UPDATE sbacsDb.Users SET Name = ?, Rights = ? WHERE User_Id = ?';
// Posts are a bit harder, since the number of arguments is variable, and we can't use % trick like GETs

var url = require('url');
var mysql = require('mysql');

var db;

module.exports = {

handleRequest : function(req,res,db_helper){
	console.log('Reached User Controller');
	
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
	var user_id = parsedRequest.query['user_id'];
	if(user_id == undefined){
		var name = parsedRequest.query['name'];
		if(name == undefined){
			name = '';
		}
		var rights = parsedRequest.query['rights'];
		if(rights == undefined){
			rights = '';
		}
		var inserts = [('%' + name), ('%' + rights)];
		queryString = mysql.format(QUERY_GET_WITHOUT_ID,inserts);
	} else {
		var inserts = [user_id];
		queryString = mysql.format(QUERY_GET_WITH_ID,inserts);
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
	// A little more to this one, leaving for later
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