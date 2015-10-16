/**
 * @file
 * Javascript functions for Feed Enhancer UI module.
 */

(function ($) {

  /**
   * Allows the re-ordering the enabled enhancer plugins.
   *
   * Copied from filter.admin.js
   */
  Drupal.behaviors.feedEnhancerPluginsStatus = {
    attach: function (context, settings) {
      $('.feed-enhancers-status-wrapper input.form-checkbox', context).once('feed-enhancer-plugin-status', function () {
        var $checkbox = $(this);
        // Retrieve the tabledrag row belonging to this enhancer plugin.
        var $row = $('#' + $checkbox.attr('id').replace(/-status$/, '-weight'), context).closest('tr');
        // Retrieve the vertical tab belonging to this enhancer plugin.
        var $tab = $('#' + $checkbox.attr('id').replace(/-status$/, '-settings'), context).data('verticalTab');

        // Bind click handler to this checkbox to conditionally show and hide the
        // filter's tableDrag row and vertical tab pane.
        $checkbox.bind('click.feedEnhancerPluginUpdate', function () {
          if ($checkbox.is(':checked')) {
            $row.show();
            if ($tab) {
              $tab.tabShow().updateSummary();
            }
          }
          else {
            $row.hide();
            if ($tab) {
              $tab.tabHide().updateSummary();
            }
          }
          // Restripe table after toggling visibility of table row.
          Drupal.tableDrag['feed-enhancers-order-table'].restripeTable();
        });

        // Attach summary for configurable items (only for screen-readers).
        if ($tab) {
          $tab.fieldset.drupalSetSummary(function (tabContext) {
            return $checkbox.is(':checked') ? Drupal.t('Enabled') : Drupal.t('Disabled');
          });
        }

        // Trigger our bound click handler to update elements to initial state.
        $checkbox.triggerHandler('click.feedEnhancerPluginUpdate');
      });
    }
  };
})(jQuery);
