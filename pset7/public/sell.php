<?php

    //configuration
    require("../includes/config.php");
    
    // if post was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        $stock = lookup($_POST["symbol"]);
        
        if($stock === false)
        {
            apologize("Please, select a symbol.");
        }
        
        $action = 'SELL';
        
        $shares = query("SELECT shares FROM stock WHERE id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        
        $cash = $stock["price"] * $shares["shares"];
        
        query("DELETE FROM stock WHERE id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        
        query("UPDATE users SET cash = cash + ? WHERE id = ?", $cash, $_SESSION["id"]);
        
        query("INSERT INTO transactions (id, symbol, shares, price, action) VALUES(?,?,?,?,?)"
              , $_SESSION["id"], $_POST["symbol"], $shares[0]["shares"], $stock["price"], $action);
        
        redirect("/");
    }
    else
    {
        $positions = query("SELECT * FROM stock WHERE id = ?", $_SESSION["id"]);
        render("sell_form.php", ["title" => "Sell", "positions" => $positions]);
    }

?>
