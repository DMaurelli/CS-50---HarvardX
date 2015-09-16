<?php

    // configuration
    require("../includes/config.php");
    
    // if post was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        $stock = lookup($_POST["symbol"]);
        
        if($stock === false)
        {
            apologize("Invalid symbol.");
        }
        else
        {
            render("result.php", ["title" => "Quote", "name" => $stock["name"], "symbol" => $stock["symbol"], "price" => $stock["price"]]);
        }
    }
    else
    {
        render("quote_form.php", ["title" => "Quote"]);
    }
    
?>
