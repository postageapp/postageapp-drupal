<?php 
  require_once('postageapp_class.inc');
  
  function process_submited_form() {
    
    # Who's going to receive this email.
    # The $to field can have the following formats:
    #
    # String:  
    #   $to = 'myemail@somewhere.com';
    #
    # Array:   
    #   $to = array('myemail@somewhere.com', 'youremail@somewhere.com', ...)
    #
    # Array with variables:
    #   $to = array(
    #     'myemail@somewhere.com'   => array('name' => 'John Smith', ...),
    #     'youremail@somewhere.com' => array('name' => 'Ann Johnson', ...),
    #     ...
    #   )
    $to = array($_POST['email'] => array('name' => $_POST['variable']));
    
    # The subject of the message
    $subject = $_POST['subject'];
    
    # Setup some headers
    $header = array(
      'From'      => 'my_test@somewhere.com',
      'Reply-to'  => 'my_test@somewhere.com'
    );
    
    # The body of the message
    $mail_body = array(
      'text/plain' => $_POST['plain_text_content'],
      'text/html' => $_POST['html_text_content']
    );
    
    # Send it all
    $response = PostageApp::mail($to, $subject, $mail_body, $header);
    return $response;
  }
  
  if (isset($_POST['email']) && $_POST['email'] !='') {
    # Processes the form if the email has been entered (see below)
    $response = process_submited_form();
  }
  
  $api_key = (POSTAGE_API_KEY == 'ENTER YOUR API KEY HERE') ? null : POSTAGE_API_KEY
  
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xml:lang='en' xmlns='http://www.w3.org/1999/xhtml'>
  <head>
    <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
    <link rel="stylesheet" href="stylesheets/main.css" type="text/css" media="screen" charset="utf-8" />
    <title>PHP Example Application for PostageApp</title>
  </head>
  
  <body>
    <div class='body_wrapper'>
      <h1> 
        PHP Example Application for PostageApp
      </h1>
      
      <div class="api_key">
        <?php if($api_key){ ?>
          API KEY: <span><?= $api_key ?></span>
        <?php } else { ?>
          Please insert your project API KEY in <span>postageapp_conf.inc<span>
        <?php } ?>
      </div>
      
      <?php if($response){ ?>
        <div class='response'>
          <pre><?= print_r ($response); ?></pre>
        </div>
      <?php } ?>
      
      <form method="post">
        <label> Subject
          <input type="text" value="Hello {{name}}" name="subject"/>
        </label>
        <label> To
          <input type="text" value="me@example.com" name="email"/>
        </label>
        <label> Name
          <input type="text" value="Tester" name="variable"/>
        </label>
        <label> HTML Content
          <textarea rows="5" name="html_text_content" cols="40">&lt;h1&gt;Hi, {{name}}&lt;/h1&gt;
&lt;p&gt;This is a test.&lt;/p&gt;
          </textarea>
        </label>
        <label> Plain Text Content
          <textarea rows="5" name="plain_text_content" cols="40">Hi, {{name}}
This is a test
          </textarea>
        </label>
        <input type="submit" value="Send Message" name="commit"/>
      </form>
      
      <ul>
        <li><a href='http://postageapp.com'>PostageApp</a></li>
        <li><a href='http://help.postageapp.com'>PostageApp Help Portal</a></li>
      </ul>
    </div>
  </body>
</html>