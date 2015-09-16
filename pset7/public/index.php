<?php

    // configuration
    require("../includes/config.php"); 
    
    $rows = query("SELECT * FROM stock WHERE id = ?", $_SESSION["id"]);
    
    $cash = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    
    $positions = [];
    
    foreach($rows as $row)
    {
        $stock = lookup($row["symbol"]);
        
        if($stock !== false)
        {
            $positions[] = [
                            "name" => $stock["name"],
                            "price" => money_format("$ %i", $stock["price"]),
                            "shares" => $row["shares"],
                            "symbol" => $row["symbol"],
                            "total" => money_format("$ %i", $stock["price"] * $row["shares"])
                          ];
        }
    }    
    
    // render portfolio
    render("portfolio.php", ["title" => "Portfolio", "positions" => $positions, "cash" => $cash[0]["cash"]]);
    
?>
