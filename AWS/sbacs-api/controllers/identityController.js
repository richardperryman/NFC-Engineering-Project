const QUERY_GET_WITH_ID = 'SELECT * FROM sbacsDb.Identities WHERE Identity_Id = ?';
const QUERY_GET_WITHOUT_ID = 'SELECT * FROM sbacsDb.Identities WHERE User_Id = ?';
const QUERY_PUT = 'INSERT INTO sbacsDb.Identities (User_Id, Name) VALUES (?, ?)';
const QUERY_DELETE = 'DELETE FROM sbacsDb.Identities WHERE Identity_Id = ?';


var url = require('url');
var mysql = require('mysql');

var db;

module.exports = {

handleRequest : function(req,res,db_helper) {
	console.log('Reached Identity Controller');
	
	db = db_helper;
	
	if(req.method === 'GET'){
		handleGet(req,res);
	} else if (req.method === 'PUT') {
		handlePut(req,res);
	} else if (req.method === 'DELETE') {
		handleDelete(req,res);
	} else {
		// Return generic message
	}
}

};



function handleGet(req,res){
	var parsedRequest = url.parse(req.url, true);
	var queryString = '';
	
	// Prepare the query to be performed
	var identity_id = parsedRequest.query['identity_id'];
	if(identity_id == undefined){
		var user_id = parsedRequest.query['user_id'];
		if(user_id == undefined){
			var inserts = ['%'];
			queryString = mysql.format(QUERY_GET_WITH_ID,inserts);
		} else {
			var inserts = [user_id];
			queryString = mysql.format(QUERY_GET_WITHOUT_ID,inserts);
		}
		
	} else {
		var inserts = [identity_id];
		queryString = mysql.format(QUERY_GET_WITH_ID,inserts);
	}
	
	console.log(queryString);
	
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