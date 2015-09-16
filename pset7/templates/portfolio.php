<ul class="nav nav-pills">
    <li><a href="quote.php">Quote</a></li>
    <li><a href="buy.php">Buy</a></li>
    <li><a href="sell.php">Sell</a></li>
    <li><a href="history.php">History</a></li>
    <li><a href="newpass.php"><strong>Change Password</strong></a></li>
    <li><a href="logout.php"><strong>Log Out</strong></a></li>
</ul>

    <table class="table table-striped">
    <thead>
        <tr>
            <th>Symbol</th>
            <th>Name</th>
            <th>Shares</th>
            <th>Price</th>
            <th>TOTAL</th>
        </tr>
    </thead>
    <tbody>
        <?php
            foreach($positions as $position)
            {
                echo "<tr>";
                echo "<td align='left'>" . $position["symbol"] . "</td>";
                echo "<td align='left'>" . $position["name"] . "</td>";
                echo "<td align='left'>" . $position["shares"] . "</td>";
                echo "<td align='left'>" . $position["price"] . "</td>";
                echo "<td align='left'>" . $position["total"] . "</td>";
                echo "</tr>";
            }
            echo "<th align='right'> CASH " . money_format("$ %i", $cash) . "</th>";
        ?>
    </tbody>

</table>
            
