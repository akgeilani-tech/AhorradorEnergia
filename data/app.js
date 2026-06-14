window.onload = function()
{
    loadStatus();

    loadRTC();
};

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

async function loadRTC()
{
    try
    {
        const response =
            await fetch(
                "/api/rtc"
            );

        const rtc =
            await response.json();

        document.getElementById(
            "rtcDateInput"
        ).value =
            rtc.year
            + "-"
            + String(
                rtc.month
            ).padStart(
                2,
                "0"
            )
            + "-"
            + String(
                rtc.day
            ).padStart(
                2,
                "0"
            );

        document.getElementById(
            "rtcTimeInput"
        ).value =
            String(
                rtc.hour
            ).padStart(
                2,
                "0"
            )
            + ":"
            + String(
                rtc.minute
            ).padStart(
                2,
                "0"
            )
            + ":"
            + String(
                rtc.second
            ).padStart(
                2,
                "0"
            );

        document.getElementById(
            "ntpServer"
        ).value =
            rtc.ntpServer;

        document.getElementById(
            "timezone"
        ).value =
            rtc.utcOffset;

        document.getElementById(
            "autoSync"
        ).checked =
            rtc.autoSync;

        if
        (
            rtc.lastSync
            >
            0
        )
        {
            const d =
                new Date(
                    rtc.lastSync
                    *
                    1000
                );

            document.getElementById(
                "lastSync"
            ).textContent =
                d.toLocaleString();
        }
        else
        {
            document.getElementById(
                "lastSync"
            ).textContent =
                "Nunca";
        }
    }
    catch(error)
    {
        console.log(
            error
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

async function restartDevice()
{
    if(
        !confirm(
            "¿Reiniciar el sistema?"
        )
    )
    {
        return;
    }

    try
    {
        await fetch(
            "/api/restart",
            {
                method:"POST"
            }
        );

        document.body.innerHTML =
            "<h2>Reiniciando...</h2>";

        setTimeout(
            function()
            {
                location.reload();
            },
            10000
        );
    }
    catch(error)
    {
        alert(
            "Error al reiniciar"
        );
    }
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

async function saveRTC()
{
    try
    {
        const date =
            document
            .getElementById(
                "rtcDateInput"
            )
            .value
            .split(
                "-"
            );

        const time =
            document
            .getElementById(
                "rtcTimeInput"
            )
            .value
            .split(
                ":"
            );

        const data =
        {
            year:
                parseInt(
                    date[0]
                ),

            month:
                parseInt(
                    date[1]
                ),

            day:
                parseInt(
                    date[2]
                ),

            hour:
                parseInt(
                    time[0]
                ),

            minute:
                parseInt(
                    time[1]
                ),

            second:
                parseInt(
                    time[2]
                ),

            ntpServer:
                document
                .getElementById(
                    "ntpServer"
                )
                .value,

            utcOffset:
                parseInt(
                    document
                    .getElementById(
                        "timezone"
                    )
                    .value
                ),

            autoSync:
                document
                .getElementById(
                    "autoSync"
                )
                .checked
        };

        const response =
            await fetch(
                "/api/rtc",
                {
                    method:
                        "POST",

                    headers:
                    {
                        "Content-Type":
                        "application/json"
                    },

                    body:
                        JSON.stringify(
                            data
                        )
                }
            );

        if
        (
            response.ok
        )
        {
            loadRTC();
        }
    }
    catch(error)
    {
        console.log(
            error
        );
    }
}

async function syncRTC()
{
    try
    {
        const response =
            await fetch(
                "/api/rtc/sync",
                {
                    method:"POST"
                }
            );

        const text =
            await response.text();

        alert(text);

        if(response.ok)
        {
            await loadRTC();
        }
    }
    catch(error)
    {
        alert(
            "Error de comunicación"
        );
    }
}

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
    updateNetworkStatus,
    3000
);

setInterval(
    updateRTC,
    1000
);

updateRTC();