// How long until keys expire
var EXPIRY_TIME = 1200000; // 20 minutes by default (in milliseconds)
var QUERY_KEY = 'SELECT * FROM sbacsDb.UserKeys WHERE User_Id = ?';
var QUERY_DELETE_KEY = 'DELETE FROM sbacsDb.UserKeys WHERE Key_Id = ?';
var QUERY_SAVE_KEY = 'INSERT INTO sbacsDb.UserKeys (User_Id, Key_Value, ExpirationTime) VALUES (?, ?, ?)';

var db = require('../extensions/db_helper.js');
var crypt = require('../extensions/crypto_helper.js');
var mysql = require('mysql');

module.exports = {

// Check if adequate authentication is provided
// Returns true if they are authenticated, false otherwise
authenticate : function(req,callback){
	// Extract needed information
	var user_id = req.headers.hmac-user;
	var hashedGiven = req.headers.hmac-content;
	if(user_id == undefined || hashedGiven == undefined){
		return callback(false);
	}
	
	// Get existing key
	getUserKey(user_id, function(key){
		if(key == undefined || key.value == undefined || hasExpired(key)){
			return callback(false);
		}
		
		// Use key to hash the message body
		var body = [];
		req.on('data', function(chunk) {
			body.push(chunk);
		}).on('end', function () {
			body = Buffer.concat(body).toString();
			// Hash the body using key
			var hashedInfo = new crypt.encryptedAuth(body,key.value);
			
			// Compare to hashedGiven
			if(hashedGiven.toString == hashedInfo.secret.toString()){
				return callback(true);
			}
		});
	
	});

},

// Gets the hmac key for a given user
// Returns undefined if no existing key
//   key will be formatted as {value:val,expiry:val}
getUserKey : function(user_id,callback){
	var inserts = [user_id];
	var queryString = mysql.format(QUERY_KEY,inserts);
	
	db.performQuery(queryString, function(err,rows,fields){
		if(!err){
			if(rows[0] != undefined){
				var key = {id:rows[0]['Key_Id'],value:rows[0]['Key_Value'],expiry:rows[0]['ExpirationTime']};
				// If the key has expired, return nothing and delete it now
				if(hasExpired(key)){
					inserts = [key.id];
					queryString = mysql.format(QUERY_DELETE_KEY, inserts);
					db.performQuery(queryString, function(err,rows,fields){});
					return callback();
				}
				return callback(key);
			} else {
				return callback();
			}
		} else {
			return callback();
		}
	});
},

// Generates a key for a user, returns the key
// Also saves it to the db
generateUserKey : function(user_id){
	var key = crypt.getSalt();
	
	// Save to db
	var expirationTime = new Date(Date.now() + EXPIRY_TIME);
	var inserts = [user_id,key,expirationTime];
	var queryString = mysql.format(QUERY_SAVE_KEY,inserts);
	db.performQuery(queryString, function(err,rows,fields){});
	
	// Return key
	return key;
}


};

function hasExpired(key){
	// Check if key has expired
	var currentTime = Date.now();
	if(currentTime >= key.expiry){
		return true;
	} else {
		return false;
	}
}


