<?php
/**
* Override the mail function
* so that we can pass the FIFTH PARAMETER.
*/
function drupal_mail_wrapper($message) {
 $mimeheaders = array();
 foreach ($message['headers'] as $name => $value) {
   $mimeheaders[] = $name .': '. mime_header_encode($value);
 }
 return mail(
   $message['to'],
   mime_header_encode($message['subject']),
   // Note: e-mail uses CRLF for line-endings, but PHP's API requires LF.
   // They will appear correctly in the actual e-mail that is sent.
   str_replace("\r", '', $message['body']),
   // For headers, PHP's API suggests that we use CRLF normally,
   // but some MTAs incorrecly replace LF with CRLF. See #234403.
   join("\n", $mimeheaders),
   ($message['from'] ? '-f' . $message['from'] : ''));
}

function mail($recipient, $subject, $mail_body, $header, $variables=NULL) {
  $content = array(
    'recipients'  => $recipient,
    'headers'     => array_merge($header, array('Subject' => $subject)),
    'variables'   => $variables,
    'uid'         => time()
  );
  if (is_string($mail_body)) {
    $content['template'] = $mail_body;
  } else {
    $content['content'] = $mail_body;
  }
  
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
    'User-Agent: PostageApp PHP ' . phpversion()
  ));   
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
  $output = curl_exec($ch);
  curl_close($ch);
  return json_decode($output);
}