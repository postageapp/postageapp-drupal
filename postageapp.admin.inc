<?php
// $Id$

/**
 * Admin settings form for the PostageApp API.
 */
function postageapp_admin_form() {
  $form = array();
  
  module_load_include('inc', 'postageapp', 'postageapp_class');

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
  
  return system_settings_form($form);
}

//'FHoI1LUaIcvb1wEy6NYmyjlSwnOXLyO9'

?>