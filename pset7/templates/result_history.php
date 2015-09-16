<table calss="table table-hover">
    <thead>
        <tr>
            <th>Date</th>
            <th>Symbol</th>
            <th>Shares</th>
            <th>Action</th>
            <th>Price</th>
        </tr>
    </thead>
    <tbody>
        <?php
            foreach($rows as $row)
            {
                echo "<tr>";
                echo "<td align='left'>" . $row["date"] . "</td>";
                echo "<td align='left'>" . $row["symbol"] . "</td>";
                echo "<td align='left'>" . $row["shares"] . "</td>";
                echo "<td align='left'>" . $row["action"] . "</td>";
                echo "<td align='left'>" . money_format("$ %i", $row["price"]) . "</td>";
                echo "</tr>";
            }
        ?>
    </tbody>
</table>
