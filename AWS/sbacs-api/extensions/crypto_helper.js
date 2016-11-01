const crypto = require('crypto');
const iterations = 100000;
const secret_length = 256;
const salt_length = 256;
const hashAlgorithm = 'sha256';

module.exports = {

encryptedAuth = function(secret, salt) {
	if (typeof salt === 'undefined') {
		salt = generateSalt();
	}
	this.salt = salt;
	this.secret = crypto.pbkdf2Sync(secret, this.salt, iterations, secret_length, hashAlgorithm);
}

};

function generateSalt() {
	var buffer = crypto.randomBytes(salt_length);
	return buffer.toString();
}