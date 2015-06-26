/*-------------------------
	Simple reset
--------------------------*/
/*@import url("http://cdnjs.cloudflare.com/ajax/libs/bootstrap-select/1.5.4/bootstrap-select.min.css")
@import url("http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css")
@import url("http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap-theme.min.css")
*/

*{
	margin:0;
	padding:0;
}


/*-------------------------
	General Styles
--------------------------*/


html{
	overflow:hidden;
}

a, a:visited {
	outline:none;
	color:#389dc1;
}

a:hover{
	text-decoration:none;
}

section, footer, header, aside{
	display: block;
}


/*-------------------------
	The main page elements
--------------------------*/

.modal-header {
    padding:9px 15px;
    border-bottom:2px solid #eee;
    background-color: #ffcd8a;
    -webkit-border-top-left-radius: 5px;
    -webkit-border-top-right-radius: 5px;
    -moz-border-radius-topleft: 5px;
    -moz-border-radius-topright: 5px;
     border-top-left-radius: 5px;
     border-top-right-radius: 5px;
 }

 .modal-body {
    padding:9px 10px;
    height: 40px;
    background-color: #ffbc63;
    -webkit-border-bottom-left-radius: 5px;
    -webkit-border-bottom-right-radius: 5px;
    -moz-border-radius-bottomleft: 5px;
    -moz-border-radius-bottomright: 5px;
     border-bottom-left-radius: 5px;
     border-bottom-right-radius: 5px;
 }



#drop {
    border: 3px dashed #ccc;
    width: 80%;
	height: 80%; 
    -webkit-border-radius: 4px;
    -moz-border-radius: 4px;
    border-radius: 4px;
    display:box;
    cursor:pointer;
    margin-left: auto ;
  	margin-right: auto ;
  	position: relative;
  	top:55px;
}

#status {
    width:450px;
    margin: 0 auto;
}

#list {
    width:450px;
    margin: 0 auto;
}

.msg-drop{
	text-align:center;
    padding:10px;
}

.thumb {
    width:33px;
    height:33px;
    float:left;
    margin:3.5px;
    border: 1px solid #ccc;
}

.fileCont{
    display:block;
    width:425px;
    height:40px;
    margin:2.5px;
    float:left;
}
.fileSize{
   text-align:right;
    margin:2.5px;
    font-size:12px;
    padding-right: 10px;
}
.fileName{
    float:left;
    padding:2.5px;
    font-weight:700;
    text-transform: capitalize;    
}
.remove{
    width:20px;
    height:40px;
    padding-top: 13px;
    font-size: 18px;
    float:left;
}

#fileBox{
    display:none;
}










.progress-bar-info {
  background-color: #ff9063;
}


body{
	font:15px/1.3 'Raleway', sans-serif;
	color: #fff;
	width:100%;
	height:100%;
	position:absolute;
	overflow:hidden;
}


#stage{
	width:100%;
	height:100%;
	position:absolute;
	top:0;
	left:0;

	transition:top 0.6s;
}

#stage > div{  /* The step divs */
	height:100%;
	position:relative;
}


#stage h1{
	font-weight:normal;
	font-size:48px;
	text-align:center;
	color:#fff;
	margin-bottom:60px;
	text-shadow: 3px 4px 0 rgba(0,0,0,0.08);
}

#stage h2{
	font-weight: normal;
	font-size: 14px;
	font-family: Arial, Helvetica, sans-serif;
	margin: -40px 0 45px;
	font-style: italic;
	text-shadow: 3px 4px 0 rgba(0,0,0,0.08);
}


#stage h4{
	font-weight:normal;
	font-size:58px;
	text-align:center;
	color:#fff;
	margin-bottom:20px;
	text-shadow: 3px 4px 0 rgba(0,0,0,0.08);
}

#stage h5{
	font-weight: normal;
	font-size: 14px;
	font-family: Arial, Helvetica, sans-serif;
	margin: 30px 0 15px;
	font-style: italic;
	text-shadow: 3px 4px 0 rgba(0,0,0,0.08);
}

#stage p2{
	font-size:11px;
	color:#ffe;
	text-shadow: 3px 4px 0 rgba(0,0,0,0.08);
}



.tos{
    width: 100px;
    height: 100px;
    position: relative;
    left: 50%;
    margin-left: -128px;
    margin-top: 25px;
    float: left;
    clear: none;
}


.how{
    width: 100px;
    height: 100px;
    position: relative;
    right: 50%;
    margin-right: -60px;
    margin-top: 25px;
    float: right;
    clear: none;
}


.content{
	position:absolute;
	text-align:center;
	left:0;
	top:40%;
	width:100%;
}

.content2{
	position:absolute;
 	margin-left: 20px;
	left:0;
	top:15%;
	width:500px;
}

.content3{
	position:absolute;
 	margin-left: 20px;
	left:0;
	top:55%;
	width:500px;
}

.proj1{
	position:absolute;
	text-align:center;
	top:5%;
	margin-left: 20px;
}

.links{


	margin-top: 35px;
	margin-left: 30px;
	display: block;
} 


.info{
	font-size: 13px;
}


.bitcoin{
    width: 100px;
    height: 100px;
    position: relative;
    left: 50%;
    margin-left: -133px;
    float: left;
    clear: none;
}

.paypal{
    width: 100px;
    height: 100px;
    position: relative;
    right: 50%;
    margin-right: -138px;
    float: right;
    clear: none;
}


button, input, optgroup, select, textarea {
  margin: 10;
  font: inherit;
  color: #c15645;
}

.content input[type=file]{
	display:none;
}

a.back{
	width: 32px;
	height: 32px;
	background: url('../img/icons.svg') no-repeat -192px 0;
	position: absolute;
	cursor: pointer;
	top: 17px;
	right: 17px;
	z-index: 10;
	opacity: 0.8;
	margin-left: -16px;
	display:none;
}

a.back:hover{
	opacity:1;
}


#stage p3{
	font-weight: normal;
	font-size: 12px;
	font-family: Arial, Helvetica, sans-serif;
	font-style: italic;
	text-shadow: 3px 4px 0 rgba(0,0,0,0.08);
}

/*-------------------------
	Conditional classes
--------------------------*/


[class*="if-"]{
	display:none;
}

body.encrypt .if-encrypt{
	display:block;
}

body.decrypt .if-decrypt{
	display:block;
}


/*-------------------------
	Button styles
--------------------------*/


.button{
	width:240px;
	height:70px;
	text-align:center;
	text-decoration: none !important;
	color:#fff !important;
	text-transform: uppercase;
	font-weight: bold;
	border-radius:1px;
	display:block;
	line-height:70px;
	box-shadow:3px 3px 0 rgba(0,0,0,0.08);
	cursor: pointer;
	font-size:18px;
	margin:10px auto;
	opacity:0.9;
}

.button:hover{
	opacity:.7;
}

.button::before{
	content:'';
	background: url(../img/icons.svg) no-repeat;
	display: inline-block;
	width: 32px;
	height: 32px;
	vertical-align: middle;
	padding-right: 13px;
}

.button.green{
	background-color:#63535b;
}

.button.magenta{
	background-color:#fabf7f;
}

.button.blue{
	background-color:#ddae7e;
}

.button.red{
	background-color: #FF725C;
}

/* Styles for specific buttons */

.button.browse{
	width:180px;
}

.button.process{
	width:190px;
}

.button.download{
	width:216px;
}

.button.encrypt::before{
	background-position: 0 0;
}

.button.decrypt::before{
	background-position: -96px 0;
}

.button.browse::before{
	background-position: -240px 0;
} 

.button.process::before{
	background-position: -142px 0;
}

.button.download:before{
	background-position:-48px 0;
}


/*-------------------------
	The steps
--------------------------*/


#step1{
	background-color:#acc18a;
}

body.encrypt #step2{
	background-color: #63535b;
}

body.decrypt #step2{
	background-color: #fcd0a1;
}

#step3{
	background-color: #63535b;
}

#step4{
	background-color:#79BD9A;
}

#step4 input[type=password]{
	background-color: #fff;
	border: none;
	padding: 8px 18px;
	line-height: 1;
	font: inherit;
	display: inline-block;
	outline: none;
	width: 300px;
	margin-bottom: 18px;
	border-radius: 2px;
	box-shadow: 3px 3px 0 rgba(0,0,0,0.05);
	font-size: 36px;
	color: #555;
}

#step5{
	background-color:#7E7F9A;
}

#step6{
	background-color:#c11b17;
}




#step1 .content{	margin-top: -140px;}
#step2 .content{	margin-top:-110px;}
#step3 .content{	margin-top:-100px;}
#step4 .content{	margin-top: -157px;}
#step5 .content{	margin-top: -100px;}
#step6 .content{	margin-top: -100px;}


