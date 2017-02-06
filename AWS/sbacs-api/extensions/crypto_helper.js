const crypto = require('crypto');
const iterations = 100000;
const secret_length = 255;
const salt_length = 255;
const hashAlgorithm = 'sha256';

module.exports = {

encryptedAuth : function(secret, salt) {
	if (typeof salt === 'undefined') {
		salt = generateSalt();
	}
	this.salt = salt;
	this.secret = crypto.pbkdf2Sync(secret, this.salt, iterations, secret_length, hashAlgorithm);
},

getSalt : function () {
	var buffer = crypto.randomBytes(salt_length);
	return buffer.toString('ascii');
}

};

function generateSalt() {
	var buffer = crypto.randomBytes(salt_length);
	return buffer.toString();
}


// HOW TO USE THIS
	// var cryptoHelper = require('./extensions/crypto_helper.js');
	// var auth = new cryptoHelper.encryptedAuth('Hello');
	// console.log('Salt: ' + auth.salt + ' Secret: ' + auth.secret);