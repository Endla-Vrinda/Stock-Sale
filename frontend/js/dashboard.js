async function loadDashboard()
{
    const response =
        await fetch(
            "/cgi-bin/dashboard.exe"
        );

    const data =
        await response.json();

    document.getElementById(
        "totalProducts"
    ).textContent =
        data.totalProducts;

    document.getElementById(
        "lowStock"
    ).textContent =
        data.lowStock;

    document.getElementById(
        "outStock"
    ).textContent =
        data.outStock;

    document.getElementById(
        "receipts"
    ).textContent =
        data.receipts;

    document.getElementById(
        "deliveries"
    ).textContent =
        data.deliveries;

    document.getElementById(
        "transfers"
    ).textContent =
        data.transfers;
}

loadDashboard();