const receiptForm =
    document.getElementById("receiptForm");

if (receiptForm)
{
    receiptForm.addEventListener(
        "submit",
        async function(event)
        {
            event.preventDefault();

            const body =
                new URLSearchParams();

            body.append(
                "supplier",
                document.getElementById("supplier").value
            );

            body.append(
                "product_id",
                document.getElementById("productId").value
            );

            body.append(
                "quantity",
                document.getElementById("quantity").value
            );

            const response =
                await fetch(
                    "/cgi-bin/operations.exe/receipt",
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
                receiptForm.reset();
                loadReceipts();
            }
        }
    );
}


const deliveryForm =
    document.getElementById("deliveryForm");

if (deliveryForm)
{
    deliveryForm.addEventListener(
        "submit",
        async function(event)
        {
            event.preventDefault();

            const body =
                new URLSearchParams();

            body.append(
                "customer",
                document.getElementById("customer").value
            );

            body.append(
                "product_id",
                document.getElementById("productId").value
            );

            body.append(
                "quantity",
                document.getElementById("quantity").value
            );

            const response =
                await fetch(
                    "/cgi-bin/operations.exe/delivery",
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
                deliveryForm.reset();
                loadDeliveries();
            }
        }
    );
}


const transferForm =
    document.getElementById("transferForm");

if (transferForm)
{
    transferForm.addEventListener(
        "submit",
        async function(event)
        {
            event.preventDefault();

            const body =
                new URLSearchParams();

            body.append(
                "product_id",
                document.getElementById("productId").value
            );

            body.append(
                "quantity",
                document.getElementById("quantity").value
            );

            body.append(
                "from_location",
                document.getElementById("fromLocation").value
            );

            body.append(
                "to_location",
                document.getElementById("toLocation").value
            );

            const response =
                await fetch(
                    "/cgi-bin/operations.exe/transfer",
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
                transferForm.reset();
                loadTransfers();
            }
        }
    );
}


const adjustmentForm =
    document.getElementById("adjustmentForm");

if (adjustmentForm)
{
    adjustmentForm.addEventListener(
        "submit",
        async function(event)
        {
            event.preventDefault();

            const body =
                new URLSearchParams();

            body.append(
                "product_id",
                document.getElementById("productId").value
            );

            body.append(
                "system_quantity",
                document.getElementById("systemQuantity").value
            );

            body.append(
                "counted_quantity",
                document.getElementById("countedQuantity").value
            );

            body.append(
                "reason",
                document.getElementById("reason").value
            );

            const response =
                await fetch(
                    "/cgi-bin/operations.exe/adjustment",
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
                adjustmentForm.reset();
                loadAdjustments();
            }
        }
    );
}


async function loadReceipts()
{
    const response =
        await fetch(
            "/cgi-bin/operations.exe/receipts"
        );

    const data =
        await response.json();

    const table =
        document.getElementById("receiptTable");

    if (!table)
    {
        return;
    }

    table.innerHTML = "";

    data.forEach(
        function(item)
        {
            const row =
                document.createElement("tr");

            row.innerHTML = `
                <td>${item.receipt_id}</td>
                <td>${item.supplier}</td>
                <td>${item.status}</td>
                <td>${item.created_at}</td>
            `;

            table.appendChild(row);
        }
    );
}


async function loadDeliveries()
{
    const response =
        await fetch(
            "/cgi-bin/operations.exe/deliveries"
        );

    const data =
        await response.json();

    const table =
        document.getElementById("deliveryTable");

    if (!table)
    {
        return;
    }

    table.innerHTML = "";

    data.forEach(
        function(item)
        {
            const row =
                document.createElement("tr");

            row.innerHTML = `
                <td>${item.delivery_id}</td>
                <td>${item.customer}</td>
                <td>${item.status}</td>
                <td>${item.created_at}</td>
            `;

            table.appendChild(row);
        }
    );
}


async function loadTransfers()
{
    const response =
        await fetch(
            "/cgi-bin/operations.exe/transfers"
        );

    const data =
        await response.json();

    const table =
        document.getElementById("transferTable");

    if (!table)
    {
        return;
    }

    table.innerHTML = "";

    data.forEach(
        function(item)
        {
            const row =
                document.createElement("tr");

            row.innerHTML = `
                <td>${item.transfer_id}</td>
                <td>${item.product_id}</td>
                <td>${item.quantity}</td>
                <td>${item.from_location}</td>
                <td>${item.to_location}</td>
                <td>${item.status}</td>
            `;

            table.appendChild(row);
        }
    );
}


async function loadAdjustments()
{
    const response =
        await fetch(
            "/cgi-bin/operations.exe/adjustments"
        );

    const data =
        await response.json();

    const table =
        document.getElementById("adjustmentTable");

    if (!table)
    {
        return;
    }

    table.innerHTML = "";

    data.forEach(
        function(item)
        {
            const row =
                document.createElement("tr");

            row.innerHTML = `
                <td>${item.adjustment_id}</td>
                <td>${item.product_id}</td>
                <td>${item.system_quantity}</td>
                <td>${item.counted_quantity}</td>
                <td>${item.difference}</td>
                <td>${item.reason}</td>
            `;

            table.appendChild(row);
        }
    );
}


async function loadMovements()
{
    const response =
        await fetch(
            "/cgi-bin/operations.exe/movements"
        );

    const data =
        await response.json();

    const table =
        document.getElementById("movementTable");

    if (!table)
    {
        return;
    }

    table.innerHTML = "";

    data.forEach(
        function(item)
        {
            const row =
                document.createElement("tr");

            row.innerHTML = `
                <td>${item.ledger_id}</td>
                <td>${item.product_id}</td>
                <td>${item.operation_type}</td>
                <td>${item.quantity}</td>
                <td>${item.reference_id}</td>
                <td>${item.created_at}</td>
            `;

            table.appendChild(row);
        }
    );
}


if (document.getElementById("receiptTable"))
{
    loadReceipts();
}

if (document.getElementById("deliveryTable"))
{
    loadDeliveries();
}

if (document.getElementById("transferTable"))
{
    loadTransfers();
}

if (document.getElementById("adjustmentTable"))
{
    loadAdjustments();
}

if (document.getElementById("movementTable"))
{
    loadMovements();
}