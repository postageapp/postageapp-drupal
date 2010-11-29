<?php
// $Id$

/**
 * Admin settings form for the PostageApp API.
 */
function postageapp_admin_form() {
  $form = array();  
  
  //override smtp_library variable so we don't have to use built in mail
  //code taken from SMTP module, Thanks!
  if (module_exists('mimemail') &&
      strpos(variable_get('smtp_library', ''), 'mimemail')) {
    // don't touch smtp_library (mimemail integration function below)
  }
  else {
    if (variable_get('postageapp_on', 0)) {
      $postageapp_path = drupal_get_filename('module', 'postageapp');
      if ($postageapp_path) {
        variable_set('smtp_library', $postageapp_path);
      }
      // If drupal can't find the path to the module, display an error.
      else {
        drupal_set_message(t("PostageApp.module error: Can't find file."), 'error');
      }
    }
    // If this module is turned off, delete the variable.
    else {
      variable_del('smtp_library');
    }
  }
  
  $form['onoff'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Install options'),
  );
  $form['onoff']['postageapp_on'] = array(
    '#type'          => 'radios',
    '#title'         => t('Turn this module on or off'),
    '#default_value' => variable_get('postageapp_on', 0),
    '#options'       => array(1 => t('On'), 0 => t('Off')),
    '#description'   => t('To uninstall this module you must turn it off here first.'),
  );
  
  $form['postageapp_account_info'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Account Info'),
  );
  $form['postageapp_account_info']['postageapp_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('PostageApp API Key'),
      '#required' => TRUE,
      '#default_value' => variable_get('postageapp_api_key', ''),
      '#description' => t('The API key for your PostageApp account. Get or generate a valid API key at your PostageApp dashboard which is available at yourusername.postageapp.com')
  );
  
  // only show the list selection forms if account info provided
  $api_key = variable_get('postageapp_api_key', FALSE);
  if ($api_key) {
    $q = new PostageApp($api_key);
    if (!$q->errorCode) {
        //do some more stuff
        // if(!defined('POSTAGE_API_KEY')) define ('POSTAGE_API_KEY', $api_key);
        // echo $api_key;
        
    } else {
      drupal_set_message(t('Could not retrieve info for postageapp for an unknown reason. Please try again later'), "error");
    }
  
  }
  
  // If an address was given, send a test e-mail message.
  $test_address = variable_get('postageapp_test_address', '');
  if ($test_address != '') {
    // Clear the variable so only one message is sent.
    variable_del('postageapp_test_address');
    global $language;
    $params['subject'] = 'Drupal test e-mail';
    $params['body']    = 'If you receive this message it means your site is capable of sending e-mail.';
    $q = drupal_mail('postageapp', 'postageapp-test', $test_address, $language, $params);    
    if ($q['result']->response->status == 'ok') {
      drupal_set_message(t('A test e-mail has been sent to @email.', array('@email' => $test_address)));
    } else {
      drupal_set_message(t('You may want to !check for any error messages.', array('!check' => l(t('check the logs'), 'admin/reports/dblog'))), 'error');
    }
  }
  $form['email_test'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Send test e-mail'),
  );
  $form['email_test']['postageapp_test_address'] = array(
    '#type'          => 'textfield',
    '#title'         => t('E-mail address to send a test e-mail to'),
    '#default_value' => '',
    '#description'   => t('Type in an address to have a test e-mail sent there.'),
  );

  $form['postageapp_debugging'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable debugging'),
    '#default_value' => variable_get('postageapp_debugging', 0),
    '#description'   => t('Checking this box will print SMTP messages from the server for every e-mail that is sent.'),
  );
  
  return system_settings_form($form);
}
?>