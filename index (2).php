<!DOCTYPE html>
<html>
    <meta http-equiv="refresh" content="3; url="http://localhost/testdate.php" />
    <head>
        <meta charset="utf-8" />
        <title>Station meteto</title>
        <style>
      

        body {
            background-image: url("https://image.noelshack.com/fichiers/2018/24/3/1528884454-fantastic-world-battle-for-sularia-volcano-533813-1920x1080.jpg");

        }

        h1 {
            font-size: 56px;
            text-align: center;
            color : red;
            font-family: Papyrus;
            text-shadow: #000000 1px 1px, #000000 -1px 1px, #000000 -1px -1px, #000000 1px -1px;
            }

        p{
            color: white;
            font-size: 24px;
            text-align:bottom;
            position:absolute;
            right:10px;
            bottom:10px;
            text-shadow: #000000 1px 1px, #000000 -1px 1px, #000000 -1px -1px, #000000 1px -1px;
        }
        .text{
            font-size: 52px;
            font-family: impact;
            text-shadow: #000000 1px 1px, #000000 -1px 1px, #000000 -1px -1px, #000000 1px -1px;
        }
        .bas{
            position: absolute;
            left: 1%;
            bottom: 1%;
        }
        .vroum{
            position: absolute;
            left: 47%;
            bottom: 0%;
        }

        </style>
    </head>
    <body>
        
       
        <h1>Station meteo de l'Apocalypse LPRO SESAM 2017/2018</h1>
        <p2 class="text">
        <div align="center">

        <?php

            $file=fopen("donnees.txt","r");
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
    </div>
    <div2 class="bas">
    <img src="https://image.noelshack.com/fichiers/2018/24/4/1528968667-logo-iut-cachan-upsaclay-bleu-sansfond.png" width="200" height="200">
    </div2>
    <div3 class="vroum">
    <img src="https://image.noelshack.com/fichiers/2018/24/4/1528969799-vroum-arnaud.png" width="300" height="300">
    </div3>
    <p>réalisé par Dorian Lamarche, Arnaud Wattebled et Florian Zanin</p>
    </body>
</html>