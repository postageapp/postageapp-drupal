<?php

if(!defined('POSTAGE_HOSTNAME')) define ('POSTAGE_HOSTNAME', 'https://api.postageapp.com');

class PostageApp
{  
  // Sends a message to Postage App
  function PostageApp($apikey) {
    $this->api_key = $apikey;
  }
  
  function get_project_info() {
    return PostageApp::post(
      'get_project_info', 
      json_encode(
        array(
          'api_key' => $this->api_key
        )
      )
    );
  }
  
  function get_account_info() {
    return PostageApp::post(
      'get_account_info', 
      json_encode(
        array(
          'api_key' => $this->api_key
        )
      )
    );
  }
  
  function get_message_receipt($UID) {
    return PostageApp::post(
      'get_message_receipt', 
      json_encode(
        array(
          'api_key' => $this->api_key,
          'uid' => $UID
        )
      )
    );
  }
  
  
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
  
  /*
    TODO: figure out how to send html and plaintext, how to know when to do so, in context of drupal
  */
  
  function mail($recipient, $subject, $mail_body, $header, $variables=NULL) {
    $content = array(
      'recipients'  => $recipient,
      'headers'     => array_merge($header, array('Subject' => $subject)),
      'variables'   => $variables,
      'uid'         => time()
    );
    /*
      TODO look at postage API, figure out how to handle templates
    */
    // if (is_string($mail_body)) {
    //   $content['template'] = $mail_body;
    // } else {
    //   $content['content'] = $mail_body;
    // }
    
    $content['content'] = $mail_body;
    
    
    return PostageApp::post(
      'send_message', 
      json_encode(
        array(
          'api_key' => $this->api_key, 
          'arguments' => $content
        )
      )
    );
  }
  
  // Makes a call to the Postage App API
  function post($api_method, $content) {
    $ch = curl_init(POSTAGE_HOSTNAME.'/v.1.0/'.$api_method.'.json');
    curl_setopt($ch, CURLOPT_POSTFIELDS,  $content);
    curl_setopt($ch, CURLOPT_HEADER, false);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json',
      'User-Agent: PostageApp Drupal (Drupal: '.(VERSION ? VERSION : '< 4.7.2').', PHP: '.phpversion().')'
    ));   
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    $output = curl_exec($ch);
    curl_close($ch);
    return json_decode($output);
  }
}

?>