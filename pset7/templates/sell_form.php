<form action="sell.php" method="post">
    <fieldset>
        <div class="form-group">
            <select class="form-control" name="symbol">
                <option value=''></option>
                <?php
                    
                    $symbols = query("SELECT * FROM stock WHERE id = ?", $_SESSION["id"]);
                    
                    foreach($symbols as $symbol)
                    {
                        echo "<option value ='" . $symbol["symbol"] . "'>" . $symbol["symbol"] . "</option>";
                    }
                
                ?>
            </select>
        </div>
        
        <div class="form-group">
            <button type="submit" class="btn btn-default">Sell</button>
        </div>
    </fieldset>
</form>
