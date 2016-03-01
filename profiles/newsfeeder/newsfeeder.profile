<?php
/**
 * @file
 * Enables modules and site configuration for a Newsfeeder site installation.
 */

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function newsfeeder_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = 'NewsFeeder';
  $form['admin_account']['account']['name']['#default_value'] = 'newsfeeder-admin';
}

/**
 * Implements hook_install_tasks().
 */
function newsfeeder_install_tasks(&$install_state) {
  $task = [];

  $task['newsfeeder_install_select_extra_features_form'] = [
    'display_name' => st('Select extra features'),
    'display' => TRUE,
    'type' => 'form',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
  ];
  $task['newsfeeder_install_install_extra_features'] = [
    'display_name' => st('Install extra features'),
    'display' => TRUE,
    'type' => 'batch',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
  ];

  return $task;
}

/**
 * Custom form builder for selecting extra features to install.
 */
function newsfeeder_install_select_extra_features_form($form, &$form_state, &$install_state) {
  drupal_set_title('Select extra features');
  $form = [];
  $form['newsfeeder_demo'] = [
    '#type' => 'checkbox',
    '#title' => st('NewsFeeder Demo'),
    '#description' => st('Demonstrate the features of the NewsFeeder.'),
  ];
  $form['newsfeeder_devel'] = [
    '#type' => 'checkbox',
    '#title' => st('Development tools'),
    '#description' => st('Collection of modules to make NewsFeeder development easier. Ex.: UI modules.'),
  ];
  $form['actions'] = ['#type' => 'actions'];
  $form['actions']['save'] = [
    '#type' => 'submit',
    '#value' => st('Save and continue'),
  ];
  return $form;
}

/**
 * Install step callback to install selected extra features.
 *
 * @param $install_state
 *   An array of information about the current installation state.
 *
 * @return array
 *   An array of batch operations.
 */
function newsfeeder_install_install_extra_features(&$install_state) {
  $batch = [
    'title' => st('Enabling and configuring extra features'),
    'operations' => [],
  ];

  // Install Newsfeeder Demo.
  if (isset($_POST['newsfeeder_demo']) && $_POST['newsfeeder_demo']) {
    $features = [
      'newsfeeder_demo',
    ];
    foreach ($features as $feature) {
      $batch['operations'][] = [
        'features_install_modules',
        [[$feature]],
      ];
      $batch['operations'][] = [
        'variable_set',
        ['site_frontpage', 'newsfeeder-demo/feeds-source-statistics'],
      ];
    }
    // Features needs to be reverted.
    $batch['operations'][] = ['features_revert', [[]]];
  }

  // Install Newsfeeder Devel.
  if (isset($_POST['newsfeeder_devel']) && $_POST['newsfeeder_devel']) {
    $batch['operations'][] = [
      'module_enable',
      [['newsfeeder_dev']],
    ];
  }

  return $batch;
}
