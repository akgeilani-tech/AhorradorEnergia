window.onload = function()
{
    loadStatus();

    loadRTC();

    loadSavedWifi();

    updateRTC();

    updateNetworkStatus();
};

function loadStatus()
{
    // if(!response.ok)
    // {
    //     throw new Error(
    //         "HTTP " + response.status
    //     );
    // }
    
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

            document.getElementById(
                "internetStatus"
            ).innerHTML =
            data.internet;

            document.getElementById(
                "firmwareVersion"
            ).innerHTML =
            data.firmware;

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

        if
        (
            rtc.lastSync
            >
            0
        )
        {
        const d =
            new Date(
                (
                    rtc.lastSync * 1000
                ) 
            );

            document.getElementById(
                "lastSync"
            ).textContent =
                d.toISOString();
            
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
            error.message
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

async function factoryReset()
{
    if(
        confirm(
            "¿Restablecer configuración?"
        )
    )
    {
        const response =
        await fetch(
            "/api/factoryreset",
            {
                method:"POST"
            }
        );
        
        if(response.ok)
        {
            alert(
            "Se restablecerá la configuración y el sistema se reiniciará, vuelva a conectarse a la red WiFi del dispositivo para configurar la conexión a internet."
            );
            restartDevice();
        }

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
            "networkRSSI"
        ).textContent =
            data.rssi;
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
    // Guardar la fecha,hora y UTC offset en el RTC
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
                    time[2] || 0
                ),

            utcOffset:
                parseInt(
                    document
                    .getElementById(
                        "timezone"
                    )
                    .value
                ),
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

async function syncNTP()
{
    try
    {
        const data =
        {
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
                    .value ===
                    "true"
        };
        const response =
            await fetch(
                "/api/rtc/sync",
                {
                    method:"POST",
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
            error.message
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
    5000
);

setInterval(
    updateRTC,
    1000
);

updateRTC();