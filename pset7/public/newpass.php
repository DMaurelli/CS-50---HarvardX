<?php

    //configuration
    require("../includes/config.php");
    
    if($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if(empty($_POST["oldpass"]))
        {
            apologize("Please enter the password you wanna change");
        }

        $rows = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
        $result = $rows[0];
        
        if(crypt($_POST["oldpass"], $result["hash"]) != $result["hash"])
        {
            apologize("The old password isn't correct");
        }

        if(empty($_POST["newpass"]))
        {
            apologize("Please enter the new password");
        }
        
        if($_POST["newpass"] != $_POST["newpassconf"])
        {
            apologize("The passwords doesn't match");
        }
        else
        {
            query("UPDATE users SET hash = ? WHERE id = ?", crypt($_POST["newpass"]), $_SESSION["id"]);
            redirect("/");
        }
    }
    else
    {
        render("newpass_form.php", ["title" => "Change Password"]);
    }

?>
