$(document).ready( function() {
	setInterval(function () {
		$.get("/test_js_func" , function(dane){
			console.log(dane)
    	});
    },1000);
});
