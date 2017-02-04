
const QUERY_GET_WITH_ID = 'SELECT * FROM sbacsDb.Users WHERE User_Id = ?';
const QUERY_GET_WITHOUT_ID = 'SELECT * FROM sbacsDb.Users WHERE Name like ? AND Rights like ?';
const QUERY_PUT = 'INSERT INTO sbacsDb.Users (Name, Rights) VALUES (?, ?)';
const QUERY_DELETE = 'DELETE FROM sbacsDb.Users WHERE User_Id = ?';
const QUERY_POST = 'UPDATE sbacsDb.Users SET Name = ?, Rights = ? WHERE User_Id = ?';

var url = require('url');
var mysql = require('mysql');
var auth_helper = require('../extensions/auth_helper.js');

var db;

module.exports = {

handleRequest : function(req,res,db_helper){
	console.log('Reached User Controller');
	
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
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var user_id = parsedRequest.query['user_id'];
	var name = parsedRequest.query['name'];
	var rights = parsedRequest.query['rights'];
	if(user_id == undefined || name == undefined || rights == undefined){
		console.log('Invalid input');
		res.writeHead(400);
		res.write('Not all parameters met.');
		res.end();
		return;
	}
	var inserts = [name,rights,user_id];
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
}

function handlePut(req,res){
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var name = parsedRequest.query['name'];
	var rights = parsedRequest.query['rights'];
	if(name == undefined || rights == undefined){
		console.log('Invalid input');
		res.writeHead(400);
		res.write('Not all parameters met.');
		res.end();
		return;
	}
	var inserts = [name,rights];
	queryString = mysql.format(QUERY_PUT,inserts);
	// Execute query, return needed results
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			var formattedOut = 'ID of created row: ' + rows.insertId;
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
}

function handleDelete(req,res){
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var user_id = parsedRequest.query['user_id'];
	if(user_id == undefined){
		console.log('Invalid input');
		res.writeHead(400);
		res.write('Not all parameters met.');
		res.end();
		return;
	}
	var inserts = [user_id];
	queryString = mysql.format(QUERY_DELETE,inserts);
	// Execute query, return needed results
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
}