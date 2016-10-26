module.exports = {

handleRequest : function(req,res) {
	console.log('Reached Access Controller');
	// See what type of request was made
	if(req.method === 'GET'){
		handleGet(req,res);
	}
}
// Note that to add a more functions, use a comma

};

function handleGet(req,res) {
	// TODO: Fill in functionality
	console.log('Reached Access Controller GET');
}