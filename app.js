'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

var myLibrary = ffi.Library(__dirname + '/libgpxparser.so', {
  "getGPXinfo": [ "string", ["string", "string"] ],
  "isValidGPXFile": [ "int", ["string", "string"] ],
  "getRouteListJSON" : [ "string", ["string", "string"] ],
  "getTrackListJSON" : [ "string", ["string", "string"] ],
  "findTracksBetweenJSON" : [ "string", [ "string", "string", "float", "float", "float", "float" ] ],
  "findRoutesBetweenJSON" : [ "string", [ "string", "string", "float", "float", "float", "float" ] ],
  "getTrackOtherData" : [ "string", [ "string", "string", "int"] ],
  "getRouteOtherData" : [ "string", [ "string", "string", "int"] ],
  "changeRouteName" : [ "bool", [ "string", "string", "int", "string"] ],
  "changeTrackName" : [ "bool", [ "string", "string", "int", "string"] ],
  "addRouteToGPX" : [ "bool", [ "string", "string", "string" ] ],
  "addWaypointToRoute" : [ "bool", [ "string", "string", "float", "float" ] ],
  "createNewGPX" : [ "bool" , [ "string", "string", "double", "string" ] ],
});

var fileArray = []; //contains all files in the upload directory upon a page load/refresh

/*app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});*/

//Respond to POST requests that upload files to uploads/ directory

function checkValidity(req, res, filename){
  let x = myLibrary.isValidGPXFile(__dirname+'/uploads/'+filename, "gpx.xsd");
  if(!Boolean(x)){
    fs.unlinkSync(__dirname+'/uploads/'+filename);
    return res.send(`<script>alert("File inputted is not of valid GPX format!"); window.location.href = "http://cis2750.socs.uoguelph.ca:38722/";</script>`);
  } else {
    return res.send(`<script>alert("File succesfully uploaded!"); window.location.href = "http://cis2750.socs.uoguelph.ca:38722/";</script>`);
  }
}

function updateTable(req, res){
  var array = [];
  fileArray.splice(0, fileArray.length);
  fs.readdir(__dirname+'/uploads/', (err, files) => {
    files.forEach(file => {
      console.log("Updating table... on file:" + file);
      var gpxInfo = myLibrary.getGPXinfo(__dirname+'/uploads/'+file, __dirname+'/gpx.xsd');
      gpxInfo = gpxInfo.replace("\n", "");
      if (gpxInfo.length > 2){
        var gpxInfoObj = JSON.parse(gpxInfo);
        array.push(gpxInfoObj);
        fileArray.push(file);
      }
    });
    return res.send({
      status : true,
      data : array,
      files : fileArray,
    });
  });
}

app.get('/setTable',function(req,res){
  updateTable(req, res);
});

app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.send(`<script>alert("No file inputted!"); window.location.href = "http://cis2750.socs.uoguelph.ca:38722/";</script>`);
  } else {

    let uploadFile = req.files.uploadFile;

    if(!(uploadFile.name.split('.').pop().toString() == "gpx")){
      return res.send(`<script>alert("Incorrect file format! Not a .gpx file"); window.location.href = "http://cis2750.socs.uoguelph.ca:38722/";</script>`);
    }

    if(fileArray.includes(uploadFile.name)){
      return res.send(`<script>alert("File has already been uploaded!"); window.location.href = "http://cis2750.socs.uoguelph.ca:38722/";</script>`);
    }

    uploadFile.mv('uploads/' + uploadFile.name, function(err) {
      if(err) {
        return res.send(`<script>alert("Error: Could not read files in uploads directory"); window.location.href = "http://cis2750.socs.uoguelph.ca:38722/";</script>`);
      }
      console.log("FILE UPLOADED: " + uploadFile.name);
      checkValidity(req, res, uploadFile.name);
    });
  }
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 



app.get('/showOtherData', function(req , res){
  let fileID = req.query.file;
  let type = req.query.type;
  let index = req.query.index;
  var result;
  console.log("Getting other data for : " + type + "in file: " + fileID);
  if (type == "Route"){
    result = myLibrary.getRouteOtherData(__dirname+'/uploads/'+fileID, __dirname+'/gpx.xsd', index);
  } else if (type == "Track"){
    result = myLibrary.getTrackOtherData(__dirname+'/uploads/'+fileID, __dirname+'/gpx.xsd', index);
  }
  result = result.replace("\n", "");
  var gpxJSON = JSON.parse(result);
  console.log("OTHER DATA FOUND : "+gpxJSON);
  return res.json({
    array : gpxJSON,
  });
});

app.get('/rename', function (req, res) {
  let fileID = req.query.file;
  let type = req.query.type;
  let index = req.query.index;
  let name = req.query.newName;
  var result;
  if (type == "Route"){
    result = myLibrary.changeRouteName(__dirname+'/uploads/'+fileID, __dirname+'/gpx.xsd', index, name);
  } else if (type == "Track"){
    result = myLibrary.changeTrackName(__dirname+'/uploads/'+fileID, __dirname+'/gpx.xsd', index, name);
  }

  if(Boolean(result)){
    return res.json({
      status : true,
      message : "Name changed succesfully!",
    });
  } else {
    return res.json({
      status : false,
      message : "Error while editing GPX file " + file,
    });
  }
});

app.get('/findPath', function(req , res){
  let p1Lon = req.query.p1Lon;
  let p1Lat = req.query.p1Lat;
  let p2Lon = req.query.p2Lon;
  let p2Lat = req.query.p2Lat;

  if(fileArray.length === 0){
    return res.json({
      status : false,
      message : "There are no files to search a path for!",
    });
  }

  if (!p1Lat || !p1Lon || !p2Lon || !p2Lat){
    return res.json({
      status : false,
      message : "Blank input!",
    });
  }

  if(p1Lat > 90 || p1Lat < -90 ||  p2Lat > 90 || p2Lat < -90){
    return res.json({
      status : false,
      message : "Out of bounds of latitude and longitude ranges",
    });
  }

  if(p1Lon > 180 || p1Lon < -180 ||  p2Lon > 180 || p2Lon < -180){
    return res.json({
      status : false,
      message : "Out of bounds of latitude and longitude ranges",
    });
  }

  var trackArray = [];
  var routeArray = [];

    for(var i = 0; i < fileArray.length; i++) {
      console.log("Searching for path in file: " + fileArray[i]);
      routeArray.push(JSON.parse(myLibrary.findRoutesBetweenJSON(__dirname+'/uploads/'+fileArray[i], __dirname+'/gpx.xsd', p1Lat, p1Lon, p2Lat, p2Lon).replace("\n", "")));
      trackArray.push(JSON.parse(myLibrary.findTracksBetweenJSON(__dirname+'/uploads/'+fileArray[i], __dirname+'/gpx.xsd', p1Lat, p1Lon, p2Lat, p2Lon).replace("\n", "")));
    }
    console.log("Routes found : "+routeArray);
    console.log("Tracks found : "+trackArray);
    return res.json({
      status : true,
      tracks : trackArray,
      routes : routeArray,
    });
});

app.get('/createRoute', function(req,res){
  let filename = req.query.filename;
  let routeName = req.query.routeName;
  let latitudeList = req.query.latitudeList;
  let longitudeList = req.query.longitudeList;
  let numPoints = req.query.numpoints;

  if(!routeName){
    routeName = null;
  }

  var x = myLibrary.addRouteToGPX(__dirname+'/uploads/'+filename, __dirname+'/gpx.xsd', routeName);
  if (!Boolean(x)){
    return res.json({
      message : "Error adding route to file",
    });
  }

  for (var i = 0; i < numPoints; i++){
    var x = myLibrary.addWaypointToRoute(__dirname+'/uploads/'+filename, __dirname+'/gpx.xsd', latitudeList[i], longitudeList[i]);
    if (!Boolean(x)){
      return res.json({
        message : "Error adding route to file",
      });
    }  
  }

  return res.json({
    message : "Route added succesfully",
    status : true,
  });

});

app.get('/createGPX', function (req, res) {
  let filename = req.query.file;
  let version = req.query.version;
  let creator = req.query.creator;
  console.log("Creating GPX: FILE : "+filename + "VERSION: "+ version + "CREATOR: "+ creator);

  if(fileArray.includes(filename)){
    return res.json({
      status : false,
      message : "Filename already exists!",
    });
  }

  var x = myLibrary.createNewGPX(__dirname+'/uploads/'+filename, __dirname+'/gpx.xsd', version, creator);

  if (Boolean(x)){
    return res.json({
      status : true,
      message : "GPX Created succesfully!",
    });
  } else {
    return res.json({
      status : false,
      message : "ERROR: GPX could not be created!",
    });
  }

});

app.get('/setDropDown', function(req , res){
  let fileID = req.query.id;
  if(fileID.length == 0){
    return res.json({
      reset : true,
    });
  }
  var routeList = JSON.parse(myLibrary.getRouteListJSON(__dirname+'/uploads/'+fileID, __dirname + "/gpx.xsd"));
  var trackList = JSON.parse(myLibrary.getTrackListJSON(__dirname+'/uploads/'+fileID, __dirname + "/gpx.xsd"));
  console.log(routeList);
  console.log(trackList);
  var routeStatus = false;
  var trackStatus = false;
  if (routeList.length > 0){
    var routeStatus = true;
  }
  if (trackList.length > 0){
    var trackStatus = true;
  }

  return res.json({
    routes : routeList,
    routestatus : routeStatus,
    tracks : trackList,
    trackstatus : trackStatus,
  });
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);