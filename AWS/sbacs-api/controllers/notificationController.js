
const QUERY_GET_WITH_ID = 'SELECT ids.name as id_name, locks.name as lock_name, locks.BelongsTo, regs.Reg_Id FROM sbacsDb.Identities as ids INNER JOIN sbacsDb.Registrations as regs ON (ids.Identity_Id = regs.Identity_Id) INNER JOIN sbacsDb.Locks as locks ON (locks.Lock_Id = regs.Lock_Id) WHERE regs.IsNew = true AND ids.User_Id = ?';
const QUERY_UPDATE_CLEAR = 'UPDATE sbacsDb.Registrations SET IsNew = FALSE WHERE Reg_Id in (?)'

var url = require('url');
var mysql = require('mysql');
var auth_helper = require('../extensions/auth_helper.js');

var db;

module.exports = {

handleRequest : function(req,res,db_helper){
	console.log('Reached Notification Controller');
	
	db = db_helper;
	
	auth_helper.authenticate(req,function(authenticated){
		if(authenticated){
			if(req.method === 'GET'){
				handleGet(req,res);
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
		res.writeHead(500);
		res.write('User Id required');
		res.end();
	} else {
		var inserts = [user_id];
		queryString = mysql.format(QUERY_GET_WITH_ID,inserts);
	}
	// Execute query, return needed results
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			var reg_ids = rows[0] ? rows[0]['Reg_Id'] : undefined;
			for (var i = 1; i < rows.length; i++) {
				reg_ids += ',' + rows[i]['Reg_Id'];
			}
			if (reg_ids) {
				var inserts = [reg_ids];
				queryString = mysql.format(QUERY_UPDATE_CLEAR, inserts);
				db.performQuery(queryString, function(err,rows,fields){
					if (err) {
						console.log("Failed to update notifications");
					}
				});
			}
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