const loginForm =
    document.getElementById("loginForm");

if (loginForm)
{
    loginForm.addEventListener(
        "submit",
        async function(event)
        {
            event.preventDefault();

            const email =
                document.getElementById("email").value;

            const password =
                document.getElementById("password").value;

            const body =
                new URLSearchParams();

            body.append(
                "email",
                email
            );

            body.append(
                "password",
                password
            );

            const response =
                await fetch(
                    "/cgi-bin/auth.exe/login",
                    {
                        method: "POST",
                        body: body
                    }
                );

            const result =
                await response.json();

            if (result.success)
            {
                localStorage.setItem(
                    "stocksenseUser",
                    email
                );

                window.location.href =
                    "dashboard.html";
            }
            else
            {
                document.getElementById(
                    "message"
                ).textContent =
                    result.message;
            }
        }
    );
}


const signupForm =
    document.getElementById("signupForm");

if (signupForm)
{
    signupForm.addEventListener(
        "submit",
        async function(event)
        {
            event.preventDefault();

            const name =
                document.getElementById("name").value;

            const email =
                document.getElementById("email").value;

            const password =
                document.getElementById("password").value;

            const body =
                new URLSearchParams();

            body.append("name", name);
            body.append("email", email);
            body.append("password", password);

            const response =
                await fetch(
                    "/cgi-bin/auth.exe/signup",
                    {
                        method: "POST",
                        body: body
                    }
                );

            const result =
                await response.json();

            document.getElementById(
                "message"
            ).textContent =
                result.message;

            if (result.success)
            {
                setTimeout(
                    function()
                    {
                        window.location.href =
                            "login.html";
                    },
                    1000
                );
            }
        }
    );
}


const forgotForm =
    document.getElementById("forgotForm");

if (forgotForm)
{
    forgotForm.addEventListener(
        "submit",
        function(event)
        {
            event.preventDefault();

            document.getElementById(
                "message"
            ).textContent =
                "OTP sent successfully! (Demo)";
        }
    );
}