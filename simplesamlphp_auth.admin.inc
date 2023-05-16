<?php
/**
 * @file
 * Admin include file for admin settings form.
 */

/**
 * Implements settings for the module.
 */
function simplesamlphp_auth_settings() {
  $config = config('simplesamlphp_auth.settings');
  global $_simplesamlphp_auth_saml_version, $base_url;

  if (!empty($_simplesamlphp_auth_saml_version)) {
    $ver = explode('.', $_simplesamlphp_auth_saml_version);
    if (!($ver[0] >= 1 && $ver[1] >= 5)) {
      backdrop_set_message(t("Please upgrade SimpleSAMLphp. You are using %ssp_version", array('%ssp_version' => $_simplesamlphp_auth_saml_version)), 'warning');
    }
  }

  $roles = user_roles(TRUE);

  $form['#config'] = 'simplesamlphp_auth.settings';
  $form['simplesamlphp_auth_grp_setup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Setup'),
    '#collapsible' => FALSE,
  );
  $form['simplesamlphp_auth_grp_setup']['activate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate authentication via SimpleSAMLphp'),
    '#default_value' => $config->get('activate'),
    '#description' => t('Checking this box before configuring the module could lock you out of Backdrop.'),
  );
  $form['simplesamlphp_auth_grp_setup']['installdir'] = array(
    '#type' => 'textfield',
    '#title' => t('Installation directory (default: /var/simplesamlphp)'),
    '#default_value' => $config->get('installdir'),
    '#description' => t('The base directory of simpleSAMLphp. Absolute path with no trailing slash.'),
  );
  $form['simplesamlphp_auth_grp_setup']['authsource'] = array(
    '#type' => 'textfield',
    '#title' => t('Authenticaton source for this SP (default: default-sp)'),
    '#default_value' => $config->get('authsource'),
    '#description' => t('The name of the source to use from /var/simplesamlphp/config/authsources.php'),
  );
  $form['simplesamlphp_auth_grp_setup']['login_link_display_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Federated login link display name'),
    '#default_value' => $config->get('login_link_display_name'),
    '#description' => t('Text to display as the link to the external federated login page. This value will be passed through the core translation layer.'),
  );
  $form['simplesamlphp_auth_grp_setup']['forcehttps'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force https for login links'),
    '#default_value' => $config->get('forcehttps'),
    '#description' => t('Should be enabled on production sites.'),
  );
  $form['simplesamlphp_auth_grp_user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Info and Syncing'),
    '#collapsible' => FALSE,
  );
  $form['simplesamlphp_auth_grp_user']['user_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Which attribute from simpleSAMLphp should be used as user's name"),
    '#default_value' => $config->get('user_name'),
    '#description' => t('Example: <i>eduPersonPrincipalName</i> or <i>displayName</i><br />If the attribute is multivalued, the first value will be used.'),
    '#required' => TRUE,
  );
  $form['simplesamlphp_auth_grp_user']['unique_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Which attribute from simpleSAMLphp should be used as unique identifier for the user'),
    '#default_value' => $config->get('unique_id'),
    '#description' => t('Example: <i>eduPersonPrincipalName</i> or <i>eduPersonTargetedID</i><br />If the attribute is multivalued, the first value will be used.'),
    '#required' => TRUE,
  );
  $form['simplesamlphp_auth_grp_user']['mailattr'] = array(
    '#type' => 'textfield',
    '#title' => t('Which attribute from simpleSAMLphp should be used as user mail address'),
    '#default_value' => $config->get('mailattr'),
    '#description' => t('Example: <i>mail</i><br />If the user attribute is multivalued, the first value will be used.'),
  );
  $form['simplesamlphp_auth_grp_user']['rolepopulation'] = array(
    '#type' => 'textarea',
    '#title' => t('Automatic role population from simpleSAMLphp attributes'),
    '#default_value' => $config->get('rolepopulation'),
    '#description' => t('A pipe separated list of rules.<br />Example: <i>roleid1:condition1|roleid2:condition2...</i> <br />For instance: <i>1:eduPersonPrincipalName,@=,uninett.no;affiliation,=,employee|2:mail,=,andreas@uninett.no</i>,3:mail,~=,andre'),
  );
  $form['simplesamlphp_auth_grp_user']['roleevaleverytime'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reevaluate roles every time the user logs in.'),
    '#default_value' => $config->get('roleevaleverytime'),
    '#description' => t('NOTE: This means users could lose any roles that have been assigned manually in Backdrop.'),
  );

  $form['simplesamlphp_auth_grp_reg'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Provisioning'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['simplesamlphp_auth_grp_reg']['registerusers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Register users (i.e., auto-provisioning)'),
    '#default_value' => $config->get('registerusers'),
    '#description' => t("Determines wether or not the module should automatically create/register new Backdrop accounts for users that authenticate using SimpleSAMLphp. Unless you've done some custom work to provision Backdrop accounts with the necessary authmap entries you will want this checked. NOTE: If unchecked each user must already have been provisioned a Backdrop account with an appropriate entry in the authmap table before logging in. Otherwise they will receive a notice and be denied access. Be aware that simply creating a Backdrop account will not create the necessary entry in the authmap table."),
  );
  $form['simplesamlphp_auth_grp_reg']['username_collision'] = array(
    '#type' => 'select',
    '#title' => t('How to handle username collisions'),
    '#options' => array(
      'abort' => t('Abort login and show error'),
      'merge' => t('Allow login and silently link accounts'),
    ),
    '#default_value' => (!empty($config->get('username_collision'))) ? $config->get('username_collision') : 'abort',
    '#description' => t("What to do when an external account conflicts with an existing local account with a matching username. The option to allow the login and link accounts should only be used in controlled circumstances when you are certain that all accounts with matching usernames represent the same individual users."),
  );
  $form['simplesamlphp_auth_grp_auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backdrop Authentication'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['simplesamlphp_auth_grp_auth']['allowsetbackdroppwd'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow SAML users to set Backdrop passwords.'),
    '#default_value' => $config->get('allowsetbackdroppwd'),
    '#description' => t('Check this box if you want to let people set passwords for their local Backdrop accounts. This will allow users to log in using either SAML or a local Backdrop account. Disabling this removes the password change fields from the user profile form.<br/>NOTE: In order for them to login using their local Backdrop password you must allow local logins with the settings below.'),
  );
  $form['simplesamlphp_auth_grp_auth']['allowdefaultlogin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow authentication with local Backdrop accounts'),
    '#default_value' => $config->get('allowdefaultlogin'),
    '#description' => t('Check this box if you want to let people log in with local Backdrop accounts (without using simpleSAMLphp). If you want to restrict this privilege to certain users you can enter the Backdrop user IDs in the field below.'),
  );
  $form['simplesamlphp_auth_grp_auth']['roles'] = array(
    '#type' => 'select',
    '#size' => 3,
    '#options' => $roles,
    '#multiple' => TRUE,
    '#title' => t('Which ROLES should be allowed to login with local accounts?'),
    '#default_value' => $config->get('roles'),
    '#description' => t('Roles that should be allowed to login without simpleSAMLphp. Examples are dev/admin roles or guest roles.'),
  );
  $form['simplesamlphp_auth_grp_auth']['users'] = array(
    '#type' => 'textfield',
    '#title' => t('Which users should be allowed to login with local accounts?'),
    '#default_value' => $config->get('users'),
    '#description' => t('Example: <i>1,2,3</i><br />A comma-separated list of user IDs that should be allowed to login without simpleSAMLphp. If left blank, all local accounts can login.'),
  );
  $form['simplesamlphp_auth_grp_auth']['logoutgotourl'] = array(
    '#type' => 'textfield',
    '#title' => t('Optionally, specify a URL for users to go to after logging out'),
    '#default_value' => $config->get('logoutgotourl'),
    '#description' => t('Example: @base_url', array('@base_url' => $base_url)),
  );

  return system_settings_form($form);
}
