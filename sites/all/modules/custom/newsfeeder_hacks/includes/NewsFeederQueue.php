<?php

/**
 * @file
 * Custom queue implementation for Newsfeeder.
 */

/**
 * It makes sure that each queue item exists only once in the queue.
 */
class NewsFeederQueue extends SystemQueue {
  /**
   * {@inheritdoc}
   */
  public function createItem($data) {
    $max_tries = 0;
    // If $data contains constraints for the maximum tries of execution, then
    // save this value to a temporary variable and unset this information from
    // the $data array.
    if (is_array($data) && isset($data['max_tries'])) {
      $max_tries = $data['max_tries'];
      unset($data['max_tries']);
    }
    $serialized_data = serialize($data);
    // Make sure that, if a queue item already exists, then the existing one
    // will be updated rather than a new queue item is created.
    return db_merge('queue')
      ->key(['name' => $this->name, 'data' => $serialized_data])
      ->fields([
        'name' => $this->name,
        'data' => $serialized_data,
        'created' => time(),
        'max_tries' => $max_tries,
      ])->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function claimItem($lease_time = 30) {
    // Claim an item by updating its expire fields. If claim is not successful
    // another thread may have claimed the item in the meantime. Therefore loop
    // until an item is successfully claimed or we are reasonably sure there
    // are no unclaimed items left.
    // Only those items were claimed which have no max_tries set or the
    // number of tries is less than the max_tries.
    while (TRUE) {
      $query = db_select('queue', 'q');
      $query->fields('q');
      $query->condition('name', $this->name)
        ->condition('expire', 0)
        ->condition(db_or()
          ->condition('max_tries', 0)
          ->condition(db_and()
            ->condition('max_tries', 0, '<>')
            ->where('tries < max_tries')
          )
        );
      $query->orderBy('tries');
      $query->orderBy('created');
      $query->orderBy('item_id');
      $query->range(0, 1);
      $item = $query->execute()->fetchAssoc();

      if ($item) {
        // Try to update the item. Only one thread can succeed in UPDATEing the
        // same row. We cannot rely on REQUEST_TIME because items might be
        // claimed by a single consumer which runs longer than 1 second. If we
        // continue to use REQUEST_TIME instead of the current time(), we steal
        // time from the lease, and will tend to reset items before the lease
        // should really expire.
        $fields = [
          'expire' => time() + $lease_time,
        ];
        // If max_tries set, then increase the number of tries too.
        if (isset($item['max_tries']) && $item['max_tries'] != 0) {
          $fields['tries'] = ++$item['tries'];
        }
        $update = db_update('queue')
          ->fields($fields)
          ->condition('item_id', $item['item_id'])
          ->condition('expire', 0);
        // If there are affected rows, this update succeeded.
        if ($update->execute()) {
          $item['data'] = unserialize($item['data']);
          return (object) $item;
        }
      }
      else {
        // No items currently available to claim.
        return FALSE;
      }
    }
  }

}
