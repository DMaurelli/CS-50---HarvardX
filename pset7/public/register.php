<?php

    // configuration
    require("../includes/config.php");
    
    // if form was submitted
    if($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // TODO
        if(empty($_POST["username"]))
        {
            apologize("You must choose an username.");
        }
        else if(empty($_POST["password"]))
        {
            apologize("You must choose a password.");
        }
        else if($_POST["password"] !== $_POST["confirmation"])
        {
            apologize("Passwords don't match.");
        }
        
        $result = query("INSERT INTO users (username, hash, cash) VALUES (?, ?, 10000.00)", $_POST["username"], crypt($_POST["password"]));
        
        if($result === false)
        {
            apologize("The username is already taken.");
        }
        
        $rows = query("SELECT LAST_INSERT_ID() AS id");
        $id = $rows[0]["id"];
        $_SESSION["id"] = $rows[0]["id"];
        redirect("/");
    }
    else
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }
    
?>
