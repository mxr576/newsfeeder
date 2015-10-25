<?php
/**
 * @file
 * Contains the definition of the Stanbol Feed Enhancer plugin.
 */

/**
 * Class StanbolFeedItemEnhancerPlugin.
 *
 * Enhancer plugin, which process the (full) content of the feed item with
 * the Apache Stanbol's Enhancer plugin.
 *
 * @see https://stanbol.apache.org/docs/trunk/components/enhancer/
 */
class StanbolFeedItemEnhancerPlugin extends FeedItemEnhancerPlugin {
  /**
   * {@inheritdoc}
   */
  public function configurationForm() {
    $form = [];
    $form['#processors'] = stanbol_enhancer_result_processor_info();
    $form['stanbol_enhancer'] = [
      '#type' => 'fieldset',
      '#title' => t('Stanbol Enhancer settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    ];
    // Set default minimum confidence level, if it has not been set.
    if (!isset($this->settings['stanbol_enhancer']['minimum_confidence_level'])) {
      $this->settings['stanbol_enhancer']['minimum_confidence_level'] = '0.5';
    }
    // Retrieve the Stanbol Enhancer configuration form.
    $form['stanbol_enhancer'] += _stanbol_enhancer_ui_entity_bundle_stanbol_enhancer_settings_generate_form('node', $this->getFeedImporterTargetNodeBundle(), $form['#processors'], $this->settings['stanbol_enhancer']);
    // Alter some generated form elements.
    $form['stanbol_enhancer']['enabled'] = [
      '#type' => 'value',
      '#value' => TRUE,
    ];
    $form['stanbol_enhancer']['process_immediately']['#default_value'] = TRUE;
    $form['stanbol_enhancer']['process_immediately']['#access'] = FALSE;
    // Mark field as required.
    $form['stanbol_enhancer']['field_to_analyze']['#states'] = [
      'required' => [
        'input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE],
      ],
    ];
    $form['stanbol_enhancer']['minimum_confidence_level']['#states'] = [
      'required' => [
        'input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE],
      ],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function configurationFormValidate(array $form, array &$form_state, array &$values) {
    _stanbol_enhancer_ui_entity_bundle_stanbol_enhancer_settings_form_validate($form['stanbol_enhancer'], $form_state, $values['stanbol_enhancer']);
    parent::configurationFormValidate($form, $form_state, $values);
  }

  /**
   * @inheritDoc
   */
  public function configurationFormSubmit(array $form, array &$form_state, array &$values) {
    // Clean out empty field values, no need to save them.
    $values['stanbol_enhancer']['mapping'] = array_filter($values['stanbol_enhancer']['mapping']);
    return parent::configurationFormSubmit($form, $form_state, $values);
  }


  /**
   * {@inheritdoc}
   */
  public static function processFeedItem(EntityMetadataWrapper $feed_item_wrapper, array $settings) {
    return stanbol_enhancer_enhance_with_entity_wrapper($feed_item_wrapper, $settings['stanbol_enhancer']);
  }

}
