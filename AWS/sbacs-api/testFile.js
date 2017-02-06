var request = require('request');
var crypt = require('./extensions/crypto_helper.js');

var hashedBody = (new crypt.encryptedAuth('','test')).secret;
console.log(hashedBody);

var encodedHmac = encodeURI(hashedBody.toString());
console.log(encodedHmac);

var headers = {
	'hmac-user' : '6',
	'hmac-content' : encodedHmac
};

var options = {
	url : 'http://127.0.0.1:3000/authenticators',
	method : 'GET',
	headers : headers
};

request(options,function(err,res,body){
	console.log(err);
	console.log(body);
	console.log(res.statusCode);
});

console.log('Test');