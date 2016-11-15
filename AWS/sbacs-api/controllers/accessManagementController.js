const QUERY_PUT = 'INSERT INTO sbacsDb.Registrations (Identity_Id, Lock_Id, ExpirationTime) VALUES (?, ?, ?)';
const QUERY_DELETE = 'DELETE FROM sbacsDb.Registrations WHERE Lock_Id = ? AND Identity_Id = ?';


var url = require('url');
var mysql = require('mysql');

var db;

module.exports = {

handleRequest : function(req,res,db_helper) {
	console.log('Reached Access Management Controller');
	
	db = db_helper;
	
	if (req.method === 'PUT') {
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




function handlePut(req,res){
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var identity_id = parsedRequest.query['identity_id'];
	var lock_id = parsedRequest.query['lock_id'];
	var expirationTime = parsedRequest.query['expirationTime'];
	if(identity_id == undefined || lock_id == undefined || expirationTime == undefined){
		console.log('Invalid input');
		res.writeHead(400);
		res.write('Not all parameters met.');
		res.end();
		return;
	}
	var inserts = [identity_id,lock_id,expirationTime];
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
	var lock_id = parsedRequest.query['lock_id'];
	var identity_id = parsedRequest.query['identity_id'];
	if(lock_id == undefined || identity_id == undefined){
		console.log('Invalid input');
		res.writeHead(400);
		res.write('Not all parameters met.');
		res.end();
		return;
	}
	var inserts = [lock_id, identity_id];
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