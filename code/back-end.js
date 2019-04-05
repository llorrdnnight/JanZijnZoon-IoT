var ws;
var wsUri = "ws:";
var loc = window.location;

console.log(loc);
if (loc.protocol === "https:") { wsUri = "wss:"; }
wsUri += "//" + loc.host + loc.pathname.replace("simple","ws/simple");

function wsConnect() {
    console.log("connect",wsUri);
    ws = new WebSocket(wsUri);
    ws.onmessage = function(msg) {

        var line = "";
        var data = msg.data;
        data = data.slice(1);
        data = data.slice(0, line.length-1);
        data = data.replace(/\"/g,"");
        data = data.replace(/\,/g,"<br>");
        line += "<p>"+ data +"</p>";

        document.getElementById('messages').innerHTML = line;

        var emptySpots = (data.match(/empty/g) || []).length;

        var amountOfSpots = "";
        amountOfSpots = "<p> Number of free spots: " + emptySpots +"</p>";
        document.getElementById('Amount_of_spots').innerHTML = amountOfSpots;

        var stringdata = "" + msg.data;
        stringdata.trim();
        var eerste = stringdata.match(".{2}(?=: empty)");
        if(stringdata.match(".{2}(?=: empty)") == null){
          eerste = "3";
        }
        eerste_line = "<p>" +"First open spot " + eerste +"</p>";
        document.getElementById('eerste').innerHTML = eerste_line;

    }
    ws.onopen = function() {
        // update the status div with the connection status
        document.getElementById('status').innerHTML = "connected";
        //ws.send("Open for data");
        console.log("connected");
    }
    ws.onclose = function() {
        // update the status div with the connection status
        document.getElementById('status').innerHTML = "not connected";
        // in case of lost connection tries to reconnect every 3 secs
        setTimeout(wsConnect,3000);
    }
}

function doit(m) {
    if (ws) { ws.send(m); }
}
