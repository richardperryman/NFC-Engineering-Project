var request = require('request');
var crypt = require('./extensions/crypto_helper.js');

var hashedBody = (new crypt.encryptedAuth('',Buffer.from('test'))).secret;
console.log(hashedBody);

var encodedHmac = encodeURI(hashedBody.toString());
console.log(encodedHmac);

var headers = {
	'hmac-user' : '6',
	'hmac-content' : encodedHmac
};

var options = {
	url : 'http://sbacs.48tdba4fch.us-west-2.elasticbeanstalk.com/authenticators',
	method : 'GET',
	headers : headers
};

request(options,function(err,res,body){
	console.log(err);
	console.log(body);
	console.log(res.statusCode);
});

console.log('Test');