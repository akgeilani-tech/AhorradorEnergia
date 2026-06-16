window.onload = function()
{
    loadStatus();

    loadRTC();
    
    loadSavedWifi();

    // loadNTPServer();
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
                "hostnameStatus"
            ).innerHTML =
            data.hostname;
            
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
        ).value =
            rtc.autoSync
            ?
            "true"
            :
            "false";

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

function loadSavedWifi()
{
    fetch(
        "/api/wifi"
    )
    .then(response => response.json())
    .then(data => {
        document.getElementById("wifiSSID").value = data.ssid;
        document.getElementById("hostname").value = data.hostname;
    });
}

// function loadNTPServer()
// {
//     fetch(
//         "/api/rtc"
//     )
//     .then(response => response.json())
//     .then(data => {
//         document.getElementById("ntpServer").value = data.ntpServer;
//         document.getElementById("lastSync").value = data.lastSync;
//         document.getElementById("autoSync").value = data.autoSync;
//     });
// }

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
        //restartDevice();
        alert(
            "Se restablecerá la configuración y el sistema se reiniciará, vuelva a conectarse a la red WiFi del dispositivo para configurar la conexión a internet."
        );

    }
}

function saveWifi()
{
    const ssid =
        document.getElementById(
            "wifiSSID"
        ).value.trim();

    const password =
        document.getElementById(
            "wifiPassword"
        ).value;

    const hostname =
        document.getElementById(
            "hostname"
        )
        .value
        .trim()
        .toLowerCase();

    if
    (
        ssid.length
        ==
        0
    )
    {
        alert(
            "El SSID es obligatorio."
        );

        return;
    }

    if
    (
        password.length
        ==
        0
    )
    {
        alert(
            "La contraseña es obligatoria."
        );

        return;
    }

    if
    (
        hostname.length
        ==
        0
    )
    {
        alert(
            "El hostname es obligatorio."
        );

        return;
    }

    if
    (
        hostname.length
        >
        31
    )
    {
        alert(
            "El hostname no puede tener más de 31 caracteres."
        );

        return;
    }

    const hostnameRegex =
    /^[a-zA-Z0-9]([a-zA-Z0-9-]{0,29}[a-zA-Z0-9])?$/;

    if
    (
        !hostnameRegex.test(
            hostname
        )
    )
    {
        alert(
            "El hostname sólo puede contener letras, números y guiones (-)."
        );

        return;
    }

    fetch(
        "/api/wifi",
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
                {
                    ssid:
                        ssid,

                    password:
                        password,

                    hostname:
                        hostname
                })
        }
    )

    .then(
        response =>
        response.text()
    )

    .then(
        text =>
        {
            alert(
                text
            );
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
        document.getElementById(
            "hostnameStatus"
        ).textContent =
            data.hostname;
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
                .value
                ===
                "true"
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