// Set up connection to DB
var mysql = require('mysql');
var pool = mysql.createPool({
	host : 'sbacsdbinstance.cpbvqdyj5xzw.us-west-2.rds.amazonaws.com',
	user : 'craigshorrocks',
	password : 'project2016jrc',
	port: 3306
});

module.exports = {

// Performs a given query, then sends results to callback function
performQuery : function (queryString, callback){
	// Get a connection to DB from the pool
	pool.getConnection(function(err,connection){
		// Make a query with that connection
		connection.query(queryString, function(err,rows,fields){
			// As soon as we have results, we can release the connection back to pool
			connection.release();
			return callback(err,rows,fields);	// Send results to callback
		});
	});
},

// A helper function that will return formatted output of
	//	the given db returned rows and fields
formatDbOutput : function (rows, fields){
	// TODO: Write this function
	
	// Return results
	// TODO: Figure out how to get column names
	var string = '';
	for(var i=0;i<rows.length;i++){
		for(val in rows[i]){
			string += rows[i][val] + ' ';
		}
		string += '\n';
	}
	return string;
}

};



// TODO: Delete this, just an example
function formatUsersDbResponse(result) {
	// TODO: Make formatting prettier (maybe use xml or json)
	var string = 'Name:   Rights: ';
	for(i=0;i<result.length;i++){
		string += '\n' + result[i].Name + '  ' + result[i].Rights;
	}
	return string;
}

