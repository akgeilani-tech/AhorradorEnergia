function loadStatus()
{
    fetch("/api/status")

    .then(
        response => response.json()
    )

    .then(
        data =>
        {
            document.getElementById(
                "systemStatus"
            ).innerHTML =
            data.system;

            document.getElementById(
                "wifiStatus"
            ).innerHTML =
            data.wifi;

            document.getElementById(
                "ipAddress"
            ).innerHTML =
            data.ip;

            document.getElementById(
                "rtcStatus"
            ).innerHTML =
            data.rtc;
        }
    );
}

window.onload = function()
{
    loadStatus();
};

function saveWifi()
{
    const ssid =
        document.getElementById(
            "wifiSSID"
        ).value;

    const password =
        document.getElementById(
            "wifiPassword"
        ).value;

    fetch(
        "/api/wifi",
        {
            method:"POST",

            headers:
            {
                "Content-Type":
                "application/json"
            },

            body:
            JSON.stringify(
            {
                ssid:ssid,
                password:password
            })
        }
    )

    .then(
        response => response.text()
    )

    .then(
        data =>
        {
            document.getElementById(
                "messageBox"
            ).innerHTML =
            data;
        }
    );
}

function restartDevice()
{
    fetch(
        "/api/restart",
        {
            method:"POST"
        }
    );
}

function factoryReset()
{
    if(
        confirm(
            "¿Restablecer configuración?"
        )
    )
    {
        fetch(
            "/api/factoryreset",
            {
                method:"POST"
            }
        );
    }
}

function saveWifi()
{
    const ssid =
        document.getElementById(
            "wifiSSID"
        ).value;

    const password =
        document.getElementById(
            "wifiPassword"
        ).value;

    fetch(
        "/api/wifi",
        {
            method: "POST",

            headers:
            {
                "Content-Type":
                "application/json"
            },

            body: JSON.stringify(
            {
                ssid: ssid,
                password: password
            })
        }
    )

    .then(
        response => response.text()
    )

    .then(
        text =>
        {
            alert(text);
        }
    );
}

async function updateNetworkStatus()
{
    try
    {
        const response =
            await fetch(
                "/api/network"
            );

        const data =
            await response.json();

        document.getElementById(
            "networkMode"
        ).textContent =
            data.mode;

        document.getElementById(
            "networkSSID"
        ).textContent =
            data.ssid;

        document.getElementById(
            "networkIP"
        ).textContent =
            data.ip;
    }
    catch(e)
    {
        console.log(
            e
        );
    }
}

setInterval(
    updateNetworkStatus,
    3000
);

updateNetworkStatus();

async function updateRTC()
{
    try
    {
        const response =
            await fetch(
                "/api/rtc"
            );

        const rtc =
            await response.json();

        const fecha =
            String(rtc.day).padStart(2,'0')
            + "/"
            + String(rtc.month).padStart(2,'0')
            + "/"
            + rtc.year;

        const hora =
            String(rtc.hour).padStart(2,'0')
            + ":"
            + String(rtc.minute).padStart(2,'0')
            + ":"
            + String(rtc.second).padStart(2,'0');

        document.getElementById(
            "rtcDate"
        ).textContent =
            fecha;

        document.getElementById(
            "rtcTime"
        ).textContent =
            hora;
    }
    catch(error)
    {
        console.log(
            error
        );
    }
}

setInterval(
    updateRTC,
    1000
);

updateRTC();