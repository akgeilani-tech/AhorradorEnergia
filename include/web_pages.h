#pragma once

const char INDEX_HTML[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<meta charset="UTF-8">

<title>
    Energy Management Controller
</title>

<style>
    body{
        font-family:Arial;
        margin:20px;
        background:#f0f0f0;
    }

    .card{
        background:white;
        padding:20px;
        border-radius:10px;
        margin-bottom:20px;
    }

    button{
        padding:10px 20px;
    }

    input{
        display:block;
        margin-bottom:10px;
    }
    #clock{

        font-size:48px;

        font-weight:bold;

        text-align:center;

        padding:20px;
    }
</style>
</head>

<script>

async function factoryReset() {

    const confirmReset =
        confirm(
            "Reset device to factory settings?"
        );

    if (!confirmReset)
        return;

    try {

        const response =
            await fetch(
                '/factoryreset',
                {
                    method: 'POST'
                }
            );

        const text =
            await response.text();

        alert(text);

    }
    catch(error) {

        alert(
            "Reset failed"
        );

        console.log(error);
    }
}

</script>

<body>

<h1>
    Energy Managment Controller
</h1>

<div class="card">
    <h2>
    Current Time
    </h2>

    <div id="clock">
    --:--
    </div>
</div>

<hr>

<h2>WiFi Configuration</h2>

<input
    type="text"
    id="wifiSSID"
    placeholder="SSID"
>

<input
    type="password"
    id="wifiPassword"
    placeholder="Password"
>

<button onclick="saveWifi()">
    Save WiFi
</button>
<hr>

<h2>Device Settings</h2>

<button class="resetButton" onclick="factoryReset()">
    Factory Reset
</button>

<script>

async function updateClock(){

    const response =
        await fetch('/time');

    const text =
        await response.text();

    document.getElementById(
        'clock'
    ).innerHTML = text;
}

setInterval(
    updateClock,
    1000
);

updateClock();


async function saveWifi() {

    const ssid =
        document.getElementById(
            'wifiSSID'
        ).value;

    const password =
        document.getElementById(
            'wifiPassword'
        ).value;

    const response =
        await fetch(
            '/savewifi',
            {
                method: 'POST',

                headers: {
                    'Content-Type':
                    'application/json'
                },

                body: JSON.stringify({

                    ssid,
                    password
                })
            }
        );

    const text =
        await response.text();

    alert(text);
}

</script>

</body>
</html>

)rawliteral";