$(document).ready( function() {
	setInterval(function () {
		$.get("/get_tasks_progress" , function(dane){
			for (var key in dane){
				$('#'+key+"_progress").attr("value", dane[key][0]);
				$('#'+key+"_progresstext").text(dane[key][0]+"%");
				$('#'+key+"_status").text(dane[key][1]);
			}
    	});
    },1000);
});