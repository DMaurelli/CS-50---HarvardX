<?php

    //configuration
    require("../includes/config.php");
    
    $rows = query("SELECT * FROM transactions WHERE id = ?", $_SESSION["id"]);
    
    render("result_history.php", ["title" => "History", "rows" => $rows]);
?>
