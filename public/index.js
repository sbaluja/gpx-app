// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function() {
    // On page-load AJAX Call 

        $.ajax({
            url: '/setTable',
            type: "get",
            dataType: 'json',
            success: function (data) {
                if(data.status == "false"){
                    window.location.href = data.redirect;
                    alert(data.message);
                } else {
                
                var trHTML = '';
                var optionHTML = '';
                if(data.data.length == 0){
                    trHTML += '<tr><td>No Files</td></tr>';
                }

                for (var i = 0; i < data.data.length; i++){
                    trHTML += 
                    '<tr><td>' + "<a href=\"" + data.files[i] + "\"" + "download" + ">" +  data.files[i] + "</a>" +
                    '</td><td>' + data.data[i].version + 
                    '</td><td>' + data.data[i].creator + 
                    '</td><td>' + data.data[i].numWaypoints + 
                    '</td><td>' + data.data[i].numRoutes + 
                    '</td><td>' + data.data[i].numTracks + 
                    '</td></tr>';  
                    optionHTML += '<option>' + data.files[i] + "</option>";
                }
                $('#FileLogPanel').append(trHTML);
                $('#GPXFiles').append(optionHTML);
                $('#GPXFiles2').append(optionHTML);
            }
            }, 
        });

        $('#GPXFiles').change(function(e){
            e.preventDefault();
            var value = $(this).val();
            $.ajax({
                url : '/setDropDown',
                type : 'get',
                dataType : 'json',
                async : false,
                data : {
                    id: value
                },
                success : function (data) {
                    var trHTML = '';
                    if(data.routestatus == true){
                        for (var i = 0; i < data.routes.length; i++){
                            trHTML += 
                            '<tr><td>' + "Route " + (i+1) +
                            '</td><td id = \"' + "Route" + (i+1) + "\"" + '>';
                            if(data.routes[i].name != "none"){
                                trHTML +=  data.routes[i].name;
                            } 
                            trHTML +=
                            '</td><td>' + data.routes[i].numPoints +
                            '</td><td>' + data.routes[i].len +
                            '</td><td>' + data.routes[i].loop + 
                            '</td></tr>';  
                        }
                    } 
                    
                    if(data.trackstatus == true){
                        for (var i = 0; i < data.tracks.length; i++){
                            trHTML += 
                            '<tr><td>' + "Track " + (i+1) + 
                            '</td><td id = \"' + "Track" + (i+1) + "\"" + '>';
                            if(data.tracks[i].name != "none"){
                                trHTML += data.tracks[i].name;
                            }
                            trHTML += 
                            '</td><td>' + data.tracks[i].numPoints +
                            '</td><td>' + data.tracks[i].len +
                            '</td><td>' + data.tracks[i].loop + 
                            '</td></tr>';  
                        }
                    }
                    var x = $('#GPXViewPanel1').find("td");
                    for (var i = 0; i < x.length; i++){
                        x[i].remove();
                    }
                    var x = $('#dialog').find("div");
                    for (var i = 0; i < x.length; i++){
                        x[i].remove();
                    }
                    $('#GPXViewPanel1').append(trHTML);
                    
                    var x = $('#GPXViewPanel1 td:nth-child(1)').map(function(){
                        return $(this).text();
                    }).get();
            
                    var popUpOptions = '<div>';
                    for (var i = 0; i < x.length; i++){
                        popUpOptions += 
                        '<input type="radio" name = "radioForm" id = \"' + x[i] + "\" + value = \"" + x[i] + "\">" +
                        '<label for=\"' + x[i] + "\">" + x[i] + '</label><br>';
                    }
                    popUpOptions += '</div>';
                    $("#dialog").prepend(popUpOptions);
                }
            });
        });
    
    $('#ShowOtherData').click(function (e){
        e.preventDefault();
        var value = $('#GPXFiles').val();
        if(!value){
            alert("No file selected");
            return;
        }
        var component = $('input[name=radioForm]:checked').val();
        if(!component){
            alert("No component selected");
            return;
        }
        var split = component.split(" ");
        var pathType = split[0];
        var pathIndex = split[1];
        
        $.ajax({
            url : '/showOtherData',
            type : 'get',
            dataType : 'json',
            async : false,
            data : {
                file : value,
                type : pathType,
                index : pathIndex,
            },
            success : function (data) {
                var string = '';
                if(data.array.length == 0){
                    alert("No other data for this component");
                } else {
                    for (var i = 0; i < data.array.length; i++){
                        string += 'name: ' + data.array[i].name + '\n' + 'value: ' + data.array[i].value + '\n';
                    }
                    alert(string);
                }
            }
        });
    });

    $('#Rename').click(function (e){
        e.preventDefault();
        var value = $('#GPXFiles').val();
        if(!value.length){
            alert("No file selected");
            return;
        }
        var component = $('input[name=radioForm]:checked').val();
        if(!component){
            alert("No component selected");
            return;
        }
        var name = $('#RenameEntry').val();
        if(!name){
            alert("No name given");
            return;
        }
        var split = component.split(" ");
        var pathType = split[0];
        var pathIndex = split[1];
        $.ajax ({
            url : '/rename',
            type : 'get',
            dataType : 'json',
            async : false,
            data : {
                file : value,
                type : pathType,
                index : pathIndex,
                newName : name,
            },
            success : function (data) {
                if (data.status == true){
                    $('#' + component.replace(" ", "")).text(name.toString());
                }
                alert(data.message);
            },
        });
    });

    $('#NumberofWaypoints').on('input', function (e){
        e.preventDefault();
        var x = $('#createRoute').find("div");
            for (var i = 0; i < x.length; i++){
                x[i].remove();
            }
        var numPoints = $('#NumberofWaypoints').val();
        if(!numPoints || numPoints < 2){
            return;
        } else {
            var form = '<div id = \"WaypointForm\">';
            for (var i = 0; i < numPoints; i++){
                form +=
                '<label for =\"Point' + (i+1) + "lat\">" + "Point " + (i+1) + " Latitude" + "</label>" +
                "<input type=\"text\" id = \"Point" + (i+1) + "lat\">" + "<br>" +
                '<label for =\"Point' + (i+1) + "lon\">" + "Point " + (i+1) + " Longitude" + "</label>" +
                "<input type=\"text\" id = \"Point" + (i+1) + "lon\">" + "<br>";
            }
            form += "</div>";
            $('#createRoute').append(form);  
        }
    });

    $('#createRoute').submit(function(e){
        e.preventDefault();
        var file = $('#GPXFiles2').val();
        if(!file){
            alert("No GPX File selected");
            return;
        }
        var name = $('#RouteName').val();
        var numPoints = $('#NumberofWaypoints').val();
        if(!numPoints){
            alert("No number of points is specified");
            return;
        }
        if(numPoints < 2){
            alert("A Route must have at least 2 points to be considered a route");
            return;
        }
        var latitudes = [];
        var longitudes = [];
        for (var i = 0; i < numPoints; i++){
            latitudes.push($('#' + 'Point' + (i+1) + 'lat').val());
            longitudes.push($('#' + 'Point' + (i+1) + 'lon').val());
            if(!longitudes[i] || !latitudes[i]){
                alert("Point specifications are missing!");
                return;
            }
        }

        $.ajax({
            type : 'get',
            dataType : 'json',
            url : '/createRoute',
            async : false,
            data : {
                filename : file,
                routeName : name,
                latitudeList : latitudes,
                longitudeList : longitudes,
                numpoints : numPoints
            },

            success : function (data) {
                alert(data.message);
                if (data.status == true){
                    window.location.href = "http://cis2750.socs.uoguelph.ca:38722/";
                }
            },
        });


        
    });

    /*$('#uploadForm').submit(function (e){
        e.preventDefault();
        var fileList = $('#uploader').prop('files');
        alert(fileList[0].name);
        $.ajax({
            type : 'post',
            dataType : 'json',
            url : '/upload',
            data : {
                files : fileList,
                uploadFile : fileList[0],
            },
            success : function (data) {
                alert(data);
                if (data.status == false){
                    alert(data.message);
                    return;
                }
            },
        });
    });*/



    $('#FindPath').submit(function(e){
        e.preventDefault();
        var p1Lon = $('#P1Longitude').val();
        var p1Lat = $('#P1Latitude').val();
        var p2Lon = $('#P2Longitude').val();
        var p2Lat = $('#P2Latitude').val();
        $.ajax({
            type : 'get',
            dataType : 'json',
            url : '/findPath',
            data : {
                p1Lon : p1Lon,
                p1Lat : p1Lat,
                p2Lon : p2Lon,
                p2Lat : p2Lat,
            },

            success : function (data) {
                var count = 1;
                var trHTML = '';
                if (data.status == false){
                    alert(data.message);
                } else {
                    for (var i = 0; i < data.routes.length; i++){
                        if(data.routes[i].length > 0){

                        for (var j = 0; j < data.routes[i].length; j++){

                        
                        trHTML += 
                        '<tr><td>' + "Route " + count++ + '</td><td>';
                        if(data.routes[i][j].name != "none"){
                            trHTML += data.routes[i][j].name;
                        }
                        trHTML +=
                        '</td><td>' + data.routes[i][j].numPoints +
                        '</td><td>' + data.routes[i][j].len +
                        '</td><td>' + data.routes[i][j].loop + 
                        '</td></tr>';  
                    }
                    }
                    }
                    count = 0;
                    for (var i = 0; i < data.tracks.length; i++){
                        if(data.tracks[i].length > 0){

                        for (var j = 0; j < data.routes[i].length; j++){
                        
                        trHTML += 
                        '<tr><td>' + "Track " + count++ + '</td><td>';
                        if(data.tracks[i][j].name != "none"){
                            trHTML += data.tracks[i][j].name;
                        } 
                        trHTML +=
                        '</td><td>' + data.tracks[i][j].numPoints +
                        '</td><td>' + data.tracks[i][j].len +
                        '</td><td>' + data.tracks[i][j].loop + 
                        '</td></tr>';  
                    }
                    }
                    }

                    var x = $('#GPXViewPanel2').find("td");
                    for (var i = 0; i < x.length; i++){
                        x[i].remove();
                    }
                    $('#GPXViewPanel2').append(trHTML);

                }
            },

        });
    });

    $('#createGPX').submit(function (e){
        e.preventDefault();
        var file = $('#Filename').val();
        var version = $('#Version').val();
        var creator = $('#Creator').val();
        var extension = file.split('.').pop().toString();

        if (!file || !version || !creator){
            alert("Please fill all fields");
            return;
        }

        if (!(extension == "gpx")){
            alert("Please input a .gpx file");
            return;
        }
        
        $.ajax({
            type : 'get',
            dataType : 'json',
            url : '/createGPX',
            data : {
                file : file,
                version : version,
                creator : creator,
            },

            success : function (data) {
                alert (data.message);
                if (data.status == true){
                    window.location.href = "http://cis2750.socs.uoguelph.ca:38722/";
                }
            },
        });
    });
    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });
});

/*

*/