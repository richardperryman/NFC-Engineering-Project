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
	res.writeHead(200);
	res.write('Generic response');
	res.end();
}

function handleDelete(req,res){
	res.writeHead(200);
	res.write('Generic response');
	res.end();
}