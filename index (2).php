<!DOCTYPE html>
<html>
    <meta http-equiv="refresh" content="3; url="http://localhost/testdate.php" />
    <head>
        <meta charset="utf-8" />
        <title>Ma page web</title>
        <style>
      

        body {
            background-image: url("https://image.noelshack.com/fichiers/2018/24/3/1528884454-fantastic-world-battle-for-sularia-volcano-533813-1920x1080.jpg");

        }

        h1 {
            color : red;
            font-family: Arial black;
        }

        p{
            color: white;
            font-size: 24px;
            text-align:bottom;
            position:absolute;
            right:10px;
            bottom:10px;
        }
        .text{
            font-size: 26px;
            font-family: impact;
        }
        .left{
            float: right;
            margin: 0 0 0 0;
        }

        </style>
    </head>
    <body>
        <img class="left" src="https://image.noelshack.com/fichiers/2018/24/3/1528883613-logo-iut-cachan-upsaclay-bleu-sansfond.png" width="250" height="250"/>
        <h1>Station meteo LPRO SESAM 2017/2018</h1>
        <p2 class="text">
        <?php

            $file=fopen("donnees.txt","r+");
            if($file)
            {
                while (($line = fgets($file,4096))!== false)
                {
                    echo '<span style ="color:green;">'.$line.'</span>';
                    echo '<br>';
                }

                fclose($file);
            }
            ?>
        </p2>
        
            
        
        
        <p>réalisé par Dorian Lamarche, Arnaud Wattebled et Florian Zanin</p>
    </body>
</html>