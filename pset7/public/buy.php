<?php

    //configuration
    require("../includes/config.php");
    
    //if form was submitted
    if($_SERVER["REQUEST_METHOD"] == "POST")
    {
        $stock = lookup($_POST["symbol"]);
        if($stock === false)
        {
            apologize("Please, enter a valid symbol.");
        }
        else if(empty($_POST["shares"]))
        {
            apologize("Please, enter the number of shares to buy.");
        }
        else if(preg_match("/^\d+$/", $_POST["shares"]) == false)
        {
            apologize("Please, enter the right number of shares.");
        }
        else
        {
            $rows = query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
            $cash = $rows[0]["cash"];
            $price = $stock["price"] * $_POST["shares"];
            $action = 'BUY';
            if($cash < $price)
            {
                apologize("You haven't enough money.");
            }
            else
            {
                $price = $stock["price"] * $_POST["shares"];
                
                query("UPDATE users SET cash = cash - ? WHERE id = ?", $price, $_SESSION["id"]);
            
                query("INSERT INTO stock (id, symbol, shares) VALUES(?,?,?) ON DUPLICATE KEY UPDATE 
                      shares = shares + VALUES(shares)", $_SESSION["id"], strtoupper($stock["symbol"]), $_POST["shares"]);
                      
                query("INSERT INTO transactions (id, symbol, shares, price, action) VALUES(?,?,?,?,?)"
                      , $_SESSION["id"], strtoupper($stock["symbol"]), $_POST["shares"], $price, $action);
            }
            redirect("/");
        }     
    }
    else
    {
        render("buy_form.php", ["title" => "Buy"]);
    }

?>
