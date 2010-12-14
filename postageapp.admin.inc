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
    $postage = new PostageApp($api_key);
    $valid_key = $postage->get_project_info();
    if ($valid_key->response->status == 'ok') {      
      $form['postageapp_project_info'] = array(
        '#type'  => 'fieldset',
        '#title' => t('<strong>Project: </strong>' . $valid_key->data->project->name),
      );      
      $form['postageapp_project_info']['url'] = array('#value' => t('<p><strong>Project URL: </strong><a href="' . $valid_key->data->project->url . '">'.$valid_key->data->project->url.'</a></p>'));
      
      $form['postageapp_project_info']['transmissions_today'] = array('#value' => t('<p><strong>Transmissions today: </strong>' . $valid_key->data->project->transmissions->today . '</p>'));
      $form['postageapp_project_info']['transmission_this_month'] = array('#value' => t('<p><strong>Transmissions this month: </strong>' . $valid_key->data->project->transmissions->this_month . '</p>'));
      $form['postageapp_project_info']['transmission_overall'] = array('#value' => t('<p><strong>Transmissions overall: </strong>' . $valid_key->data->project->transmissions->overall . '</p>'));
      $form['postageapp_project_info']['users_label'] = array('#value' => t('<p><strong>Users:</strong></p>'));
      $users = '';
      foreach ($valid_key->data->project->users as $key => $val) {
        $users[$key] = $val;
        $users_string .= '<p>' . $val . ': ' . $key . '</p>';
      }
      krumo($users);
      
      $form['postageapp_project_info']['users'] = array('#value' => t($users_string));
      variable_set('postageapp_users', $users);
      
      if (!(variable_get('postageapp_test_email_sent', FALSE))) {
        variable_set('postageapp_test_email_sent', TRUE);
        drupal_set_message(t('Your API key is valid.  Welcome to PostageApp.'), "notice");
      }
      
      $form['postageapp_debugging'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable debugging'),
        '#default_value' => variable_get('postageapp_debugging', 0),
        '#description'   => t('Checking this box will send flag messages as testing messages (currently in development).'),
      );
      $form['send_test_email'] = array(
        '#type' => 'button',
        '#value' => t('Send Test Email'),
        '#weight' => 19
      );
    } else {
      drupal_set_message(t($valid_key->response->message), "error");
    }
  }
  // $form['#submit']['postageapp_admin_form_submit'] = array();
  return system_settings_form($form);
  // return $form;
}

function postageapp_admin_form_validate($form, &$form_state) {
  if ($form['#post']['op'] == 'Send Test Email') postageapp_send_test_email();
}

// function postageapp_admin_form_alter($form, &$form_state) {
//   $form['#submit'][] = 'postageapp_admin_form_submit';
// }

// function postageapp_admin_form_submit($form_id, &$form_state) {
//   if ($form['#post']['op'] == 'Send Test Email') postageapp_send_test_email($form['#post']);
// }



function postageapp_send_test_email() {
  $test_address = variable_get('postageapp_users', '');
  // krumo($addresses);
    
  global $language;
  $params['subject'] = 'PostageApp test e-mail';
  $params['body']    = 'If you receive this message it means your site is capable of sending e-mail using PostageApp.';
  $q = drupal_mail('postageapp', 'postageapp-test', $test_address, $language, $params);    
  if ($q['result']->response->status == 'ok') {
    drupal_set_message(t('A test e-mail has been sent to @email.', array('@email' => $test_address)));
  } else {
    drupal_set_message(t('You may want to !check for any error messages.', array('!check' => l(t('check the logs'), 'admin/reports/dblog'))), 'error');
  }
}

/**
 * Implementation of postageapp_mail().
 */
function postageapp_mail($key, &$message, $params) {
  if ($key == 'postageapp-test') {
    $message['subject'] = $params['subject'];
    $message['body']    = $params['body'];
  }
}  //  End of postageapp_mail().
?>